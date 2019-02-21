#**************************  M a k e f i l e ********************************
#
#         Author: ds
#          $Date: 2007/08/09 09:15:58 $
#      $Revision: 1.3 $
#
#    Description: makefile descriptor for ID library
#
#                 variant id_sw for swapped access
#
#-----------------------------------------------------------------------------
#   Copyright (c) 1999-2019, MEN Mikro Elektronik GmbH
#*****************************************************************************
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


MAK_NAME=id_sw

MAK_LIBS=

MAK_SWITCH=$(SW_PREFIX)ID_SW \
		   $(SW_PREFIX)MAC_MEM_MAPPED

MAK_INCL=$(MEN_MOD_DIR)/id_var.h \
         $(MEN_INC_DIR)/men_typs.h \
         $(MEN_INC_DIR)/dbg.h \
         $(MEN_INC_DIR)/oss.h \
         $(MEN_INC_DIR)/maccess.h \
         $(MEN_INC_DIR)/modcom.h

MAK_INP1=c_drvadd$(INP_SUFFIX)
MAK_INP2=microwire_port$(INP_SUFFIX)
MAK_INP3=usmrw$(INP_SUFFIX)

MAK_INP=$(MAK_INP1)\
		$(MAK_INP2)\
		$(MAK_INP3)


