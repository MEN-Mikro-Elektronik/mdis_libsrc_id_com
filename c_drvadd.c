/*********************  P r o g r a m  -  M o d u l e **********************/
/*!
 *         \file c_drvadd.c
 *      Project: common lib for m-modules
 *
 *       \author kp
 *
 *        \brief Handling Module-Identification (EEPROM)
 *               MICROWIRE Protocoll
 *
 *     Required: none
 *     Switches: none
 */
 /*---------------------------[ Public Functions ]----------------------------
 *
 * int m_mread(addr,buff)            multiple read i=0..15
 * int m_mwrite(addr,buff)           multiple write i=0..15
 * int m_read(addr,index)            single read i
 * int m_write(addr,index,data)      single write i
 * int m_getmodinfo(base,modtype,    get module information
 *                  devid,devrev,
 *                  devname)
 *
 *---------------------------------------------------------------------------
 * Copyright (c) 1993-2019, MEN Mikro Elektronik GmbH
 ******************************************************************************/
/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "id_var.h"			/* defines variants */
#include <MEN/men_typs.h>
#include <MEN/dbg.h>
#include <MEN/oss.h>
#include <MEN/maccess.h>
#include <MEN/modcom.h>

static const char IdentString[]=MENT_XSTR(MAK_REVISION);

#define DELAY   20	                /* m_clock's delay time */

/* id defines */
#define MOD_ID_MAGIC	0x5346  /* M-Module id prom magic word */
#define MOD_ID_MS_MASK	0x5300	/* mask to indicate MSxx M-Module */
#define MOD_ID_N_MASK	0x7D00	/* mask to indicate MxxN M-Module */

/*--- instructions for serial EEPROM ---*/
#define     _READ_   0x80    /* read data */
#define     EWEN    0x30    /* enable erase/write state */
#define     ERASE   0xc0    /* erase cell */
#define     _WRITE_  0x40    /* write data */
#define     ERAL    0x20    /* chip erase */
#define     WRAL    0x10    /* chip write */
#define     EWDS    0x00    /* disable erase/write state */

#define     T_WP    10000   /* max. time required for write/erase (us) */

/* bit definition */
#define B_DAT	0x01				/* data in-;output		*/
#define B_CLK	0x02				/* clock				*/
#define B_SEL	0x04				/* chip-select			*/

/* A08 register address */
#define     MODREG  0xfe

/*--- K&R prototypes ---*/
static int _write( U_INT32_OR_64 base, u_int8 index, u_int16 data );
static int _erase( U_INT32_OR_64 base, u_int8 index );
static void _opcode( U_INT32_OR_64 base, u_int8 code );
static void _select( U_INT32_OR_64 base );
static void _deselect( U_INT32_OR_64 base );
static int _clock( U_INT32_OR_64 base, u_int8 dbs );
static void _delay( void );
static void _xtoa( u_int32 val, u_int32 radix, char *buf );

/******************************* m_mread ***********************************/
/**   Read all contents (words 0..15) from EEPROM at 'base'.
 *
 *---------------------------------------------------------------------------
 *  \param addr			\IN base address pointer
 *  \param buff			\INOUT user buffer (16 words)
 *  \return   0=ok, 1=error
 *
 ****************************************************************************/
int m_mread( u_int8   *addr, u_int16  *buff )
{

    register u_int8    index;

    for(index=0; index<16; index++)
        *buff++ = (u_int16)m_read( (U_INT32_OR_64)addr, index);
    return 0;
}

/******************************* m_mwrite **********************************/
/**   Write all contents (words 0..15) into EEPROM at 'base'.
 *
 *---------------------------------------------------------------------------
 *  \param addr		\IN base address pointer
 *  \param buff		\IN user buffer (16 words)
 *  \return   0=ok, 1=error
 *
 ****************************************************************************/
int m_mwrite( u_int8  *addr, u_int16 *buff )
{
    register u_int8    index;

    for(index=0; index<16; index++)
        if( m_write(addr,index,*buff++) )
            return 1;
    return 0;
}


/******************************* m_write ***********************************/
/**   Write a specified word into EEPROM at 'base'.
 *
 *---------------------------------------------------------------------------
 *  \param addr			\IN base address pointer
 *  \param index		\IN index to write (0..15)
 *  \param data			\IN word to write
 *  \return   0=ok; 1=write err; 2=verify err
 *
 ***************************************************************************/
int m_write( u_int8 *addr, u_int8  index, u_int16 data )
{
    if( _erase( (U_INT32_OR_64)addr, index ))              /* erase cell first */
        return 3;

    return _write( (U_INT32_OR_64)addr, index, data );
}

/******************************* m_read ************************************/
/**   Read a specified word from EEPROM at 'base'.
 *
 *---------------------------------------------------------------------------
 *  \param base			\IN base address pointer
 *  \param index		\IN index to read (0..15)
 *  \return   read word
 *
 ****************************************************************************/
int m_read( U_INT32_OR_64 base, u_int8 index )
{
    register u_int16    wx;                 /* data word    */
    register int        i;                  /* counter      */

    _opcode(base, (u_int8)(_READ_+index) );
    for(wx=0, i=0; i<16; i++)
        wx = (u_int16)((wx<<1)+_clock(base,0));
    _deselect(base);

    return(wx);
}

/******************************* m_getmodinfo ******************************/
/**   Get module information.
 *
 *                The function reads the magic-id, mod-id, layout-rev and
 *                product-variant from the EEPROM, evaluates these parameters
 *                and provide the module information for the caller.
 *
 *                1) If the four read values are equal, then we assume that
 *                   the EEPROM is not present or is invalid.
 *                   In this case, the function returns with the following
 *                   parameters:
 *                   - modtype = 0.
 *             		 - devid  = 0xffffffff
 *                   - devrev = 0xffffffff
 *                   - devname = '\\0'
 *
 *                2) If the four read values are different, the function
 *                   gives the following parameters to the caller:
 *             		 - devid  = (magic-id   << 16) | mod-id
 *                   - devrev = (layout-rev << 16) | product-variant
 *
 *                2a) If magic-id = 0x5346 the function returns with:
 *                    - modtype = MODCOM_MOD_MEN
 *					  - devname = "<prefix><decimal mod-id><suffix>"
 *					              - if (mod-id & 0xFF00) == 0x5300 then
 *                                  - prefix="MS"
 *					              - else
 *                                  - prefix="M"
 *					                - if (mod-id & 0xFF00) = 0x7D00 then
 *                                    - suffix="N"
 *
 *                                e.g. M34, MS9, M45N
 *
 *                2b) If magic-id <> 0x5346 the function returns with:
 *                  - modtype = MODCOM_MOD_THIRD
 *                  - devname = '\\0'
 *
 *---------------------------------------------------------------------------
 *  \param base			\IN	base address pointer
 *  \param modtype		\OUT module type (0, MODCOM_MOD_MEN, MODCOM_MOD_THIRD)
 *  \param devid		\OUT device id
 *  \param devrev		\OUT device revision
 *  \param devname		\OUT device name
 *  \return    0=ok, 1=error
 *
 ****************************************************************************/
int m_getmodinfo(
	U_INT32_OR_64 base,
	u_int32 *modtype,
	u_int32 *devid,
	u_int32 *devrev,
	char    *devname )
{
	u_int16	magic, modid, layout, variant;
	u_int8	addSuffix = FALSE;
	char	*bufptr = devname;

	/* set defaults */
	*devid   = 0xffffffff;
	*devrev  = 0xffffffff;
	*devname = '\0';

	/* read data from eeprom */
	magic   = (u_int16)m_read(base, 0);
	modid   = (u_int16)m_read(base, 1);
	layout  = (u_int16)m_read(base, 2);
	variant	= (u_int16)m_read(base, 8);

	/*------------------------------+
	| M-Module without id-prom data |
	+------------------------------*/
	/*
	 * If all read data are equal then we assume there is a M-Module
	 * without id-prom or without valid id-prom data.
	 */
	if( (magic  == modid) &&
		(layout == variant) &&
		(magic  == layout) ){

		*modtype = 0;
		return 0;
	}

	/*------------------------------+
	| M-Module with id-prom data    |
	+------------------------------*/
	else {

		/* build devid and devrev */
		*devid   = (magic  << 16) | modid;
		*devrev  = (layout << 16) | variant;

		/*------------------------------+
		| VITA conform M-Module         |
		+------------------------------*/
		/*
		 * If we got the right magic-id then
		 * we assume there is a VITA conform M-Module.
		 */
		if( magic == MOD_ID_MAGIC ){

			*modtype = MODCOM_MOD_MEN;

			/*
			 * build device name
			 */

			*bufptr = 'M';
			bufptr++;

			/* MSxx M-Module? */
			if( (modid & 0xFF00) == MOD_ID_MS_MASK ){
				*bufptr = 'S';
				bufptr++;
				modid &= 0x00FF;
			}
			/* MxxN M-Module? */
			else if( (modid & 0xFF00) == MOD_ID_N_MASK ){
				addSuffix = TRUE;
				modid &= 0x00FF;
			}

			/* add modid */
			_xtoa( modid, 10, bufptr );

			/* MxxN M-Module */
			if( addSuffix ){
				bufptr = devname;
				while( *bufptr != '\0' )
					bufptr++;
				*bufptr++ = 'N';
				*bufptr = '\0';
			}
		}

		/*------------------------------+
		| other M-Module                |
		+------------------------------*/
		/*
		 * Not the right magic-id
		 */
		else{
			*modtype = MODCOM_MOD_THIRD;
		}
	}

	return 0;
}

/******************************* _xtoa *************************************/
/**   Converts an u_int32 to a character string.
 *
 *---------------------------------------------------------------------------
 *  \param radix		\IN	base to convert into
 *  \param val			\IN	number to be converted
 *  \param buf			\IN	ptr to buffer to place result
 *	\param buf			\OUT computed string
 *  Globals....:  ---
 ****************************************************************************/
static void _xtoa( u_int32 val, u_int32 radix, char *buf )
{
	char	*p;           /* pointer to traverse string */
    char	*firstdig;    /* pointer to first digit */
    char	temp;         /* temp char */
    u_int32 digval;       /* value of digit */

    p = buf;

	/* save pointer to first digit */
    firstdig = p;

    do {
        digval = (u_int32) (val % radix);
        val /= radix; /* get next digit */

        /* convert to ascii and store */
        if (digval > 9)
            *p++ = (char) (digval - 10 + 'a');  /* a letter */
        else
            *p++ = (char) (digval + '0');       /* a digit */
    } while (val > 0);

    /* terminate string; p points to last digit */
    *p-- = '\0';

    /* reverse buffer */
    do {
        temp = *p;
        *p = *firstdig;
        *firstdig = temp;   /* swap *p and *firstdig */
        --p;
        ++firstdig;         /* advance to next two digits */
    } while (firstdig < p); /* repeat until halfway */
}

/******************************* _write ***********************************/
/**   Write a specified word into EEPROM at 'base'.
 *
 *---------------------------------------------------------------------------
 *	\param base			\IN base address pointer
 *	\param index		\IN index to write (0..63)
 *  \param data			\IN word to write
 *  \return   0=ok 1=write err 2=verify err
 *
 ***************************************************************************/
static int _write( U_INT32_OR_64 base, u_int8 index, u_int16 data )
{
    register int    i,j;                    /* counters     */

    _opcode(base,EWEN);                     /* write enable */
    _deselect(base);                        /* deselect     */

    _opcode(base, (u_int8)(_WRITE_+index) );             /* select write */
    for(i=15; i>=0; i--)
        _clock(base,(u_int8)((data>>i)&0x01));        /* write data   */
    _deselect(base);                        /* deselect     */

    _select(base);
    for(i=T_WP; i>0; i--)                   /* wait for low */
    {   if(!_clock(base,0))
            break;
        _delay();
    }
    for(j=T_WP; j>0; j--)                   /* wait for high*/
    {   if(_clock(base,0))
            break;
        _delay();
    }

    _opcode(base, EWDS);                    /* write disable*/
    _deselect(base);                        /* disable      */

    if((i==0) || (j==0))                    /* error ?      */
        return 1;                           /* ..yes */

    if( data != m_read(base,index) )        /* verify data  */
        return 2;                           /* ..error      */

    return 0;                               /* ..no         */
}

/******************************* _erase ***********************************/
/**   Erase a specified word into EEPROM
 *
 *---------------------------------------------------------------------------
 *	\param base			\IN base address pointer
 *	\param index		\IN index to write (0..15)
 *  \return   0=ok 1=error
 *
 ***************************************************************************/
static int _erase( U_INT32_OR_64 base, u_int8 index )
{
    register int    i,j;                    /* counters     */

    _opcode(base,EWEN);                     /* erase enable */
    for(i=0;i<4;i++) _clock(base,0);
    _deselect(base);                        /* deselect     */

    _opcode(base,(u_int8)(ERASE+index) );              /* select erase */
    _deselect(base);                        /* deselect     */

    _select(base);
    for(i=T_WP; i>0; i--)                   /* wait for low */
    {   if(!_clock(base,0))
            break;
        _delay();
    }

    for(j=T_WP; j>0; j--)                   /* wait for high*/
    {   if(_clock(base,0))
            break;
        _delay();
    }

    _opcode(base,EWDS);                     /* erase disable*/
    _deselect(base);                        /* disable      */

    if((i==0) || (j==0))                    /* error ?      */
        return 1;
    return 0;
}

/******************************* _opcode ***********************************/
/**   Output opcode with leading startbit
 *
 *---------------------------------------------------------------------------
 *	\param base			\IN base address pointer
 *	\param code			\IN opcode to write
 *
 ***************************************************************************/
static void _opcode( U_INT32_OR_64 base, u_int8 code )
{
    register int i;

    _select(base);
    _clock(base,1);                         /* output start bit */

    for(i=7; i>=0; i--)
        _clock(base,(u_int8)((code>>i)&0x01) );        /* output instruction code  */
}


/*----------------------------------------------------------------------
 * LOW-LEVEL ROUTINES FOR SERIAL EEPROM
 *--------------------------------------------------------------------*/

/******************************* _select ***********************************/
/**   Select EEPROM:
 *                 output DI/CLK/CS low
 *                 delay
 *                 output CS high
 *                 delay
 *---------------------------------------------------------------------------
 *  \param base			\IN base address pointer
 *
 ***************************************************************************/
static void _select( U_INT32_OR_64 base )
{
    MWRITE_D16( base, MODREG, 0 );			/* everything inactive */
    _delay();
    MWRITE_D16( base, MODREG, B_SEL );		/* select high */
    _delay();
}

/******************************* _deselect *********************************/
/**   Deselect EEPROM
 *                 output CS low
 *---------------------------------------------------------------------------
 *  \param base			\IN base address pointer
 *
 ***************************************************************************/
static void _deselect( U_INT32_OR_64 base )
{
    MWRITE_D16( base, MODREG, 0 );			/* everything inactive */
}


/******************************* _clock ***********************************/
/**   Output data bit:
 *                 output clock low
 *                 output data bit
 *                 delay
 *                 output clock high
 *                 delay
 *                 return state of data serial eeprom's DO - line
 *                 (Note: keep CS asserted)
 *---------------------------------------------------------------------------
 *  \param base			\IN base address pointer
 *	\param dbs			\IN	data bit to send
 *  \return state of DO line
 *
 ***************************************************************************/
static int _clock( U_INT32_OR_64 base, u_int8 dbs )
{
    MWRITE_D16( base, MODREG, dbs|B_SEL );  /* output clock low */
                                            /* output data high/low */
    _delay();                               /* delay    */

    MWRITE_D16( base, MODREG, dbs|B_CLK|B_SEL );  /* output clock high */
    _delay();                               /* delay    */

    return( MREAD_D16( base, MODREG) & B_DAT );  /* get data */
}

/******************************* _delay ************************************/
/**   Delay (at least) one microsecond
 *---------------------------------------------------------------------------
 *
 ***************************************************************************/
static void _delay( void )
{
    register volatile int i,n;

    for(i=DELAY; i>0; i--)
        n=10*10;
}









