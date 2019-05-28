<?xml version='1.0' encoding='ISO-8859-1' standalone='yes'?>
<tagfile>
  <compound kind="page">
    <filename>index</filename>
    <title></title>
    <name>index</name>
  </compound>
  <compound kind="file">
    <name>c_drvadd.c</name>
    <path>/opt/menlinux/LIBSRC/ID/COM/</path>
    <filename>c__drvadd_8c</filename>
    <member kind="define">
      <type>#define</type>
      <name>DELAY</name>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MOD_ID_MAGIC</name>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MOD_ID_MS_MASK</name>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MOD_ID_N_MASK</name>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_READ_</name>
      <anchor>a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>EWEN</name>
      <anchor>a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERASE</name>
      <anchor>a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_WRITE_</name>
      <anchor>a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERAL</name>
      <anchor>a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WRAL</name>
      <anchor>a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>EWDS</name>
      <anchor>a10</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>T_WP</name>
      <anchor>a11</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>B_DAT</name>
      <anchor>a12</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>B_CLK</name>
      <anchor>a13</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>B_SEL</name>
      <anchor>a14</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MODREG</name>
      <anchor>a15</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>m_mread</name>
      <anchor>a25</anchor>
      <arglist>(u_int8 *addr, u_int16 *buff)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>m_mwrite</name>
      <anchor>a26</anchor>
      <arglist>(u_int8 *addr, u_int16 *buff)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>m_write</name>
      <anchor>a27</anchor>
      <arglist>(u_int8 *addr, u_int8 index, u_int16 data)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>m_read</name>
      <anchor>a28</anchor>
      <arglist>(U_INT32_OR_64 base, u_int8 index)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>m_getmodinfo</name>
      <anchor>a29</anchor>
      <arglist>(U_INT32_OR_64 base, u_int32 *modtype, u_int32 *devid, u_int32 *devrev, char *devname)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>id_doc.c</name>
    <path>/opt/menlinux/LIBSRC/ID/COM/</path>
    <filename>id__doc_8c</filename>
  </compound>
  <compound kind="file">
    <name>microwire_port.c</name>
    <path>/opt/menlinux/LIBSRC/ID/COM/</path>
    <filename>microwire__port_8c</filename>
    <class kind="struct">MCRW_HANDLE</class>
    <member kind="define">
      <type>#define</type>
      <name>MCRW_COMPILE</name>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_READ_</name>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>EWEN</name>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERASE</name>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_WRITE_</name>
      <anchor>a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERAL</name>
      <anchor>a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WRAL</name>
      <anchor>a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>EWDS</name>
      <anchor>a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>T_WP</name>
      <anchor>a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>B_DAT</name>
      <anchor>a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>B_CLK</name>
      <anchor>a10</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>B_SEL</name>
      <anchor>a11</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>u_int32</type>
      <name>MCRW_PORT_Init</name>
      <anchor>a28</anchor>
      <arglist>(MCRW_DESC_PORT *descP, void *osHdl, void **mcrwHdlP)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>usmrw.c</name>
    <path>/opt/menlinux/LIBSRC/ID/COM/</path>
    <filename>usmrw_8c</filename>
    <member kind="define">
      <type>#define</type>
      <name>DELAY</name>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>USM_ID_MAGIC</name>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_READ_USM</name>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_WRITE_USM</name>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>B_DAT</name>
      <anchor>a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>B_CLK</name>
      <anchor>a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>B_SEL</name>
      <anchor>a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MODREG</name>
      <anchor>a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>usm_mread</name>
      <anchor>a9</anchor>
      <arglist>(u_int8 *addr, u_int16 *buff)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>usm_mwrite</name>
      <anchor>a10</anchor>
      <arglist>(u_int8 *addr, u_int16 *buff)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>usm_write</name>
      <anchor>a11</anchor>
      <arglist>(u_int8 *addr, u_int8 index, u_int16 data)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>usm_read</name>
      <anchor>a12</anchor>
      <arglist>(U_INT32_OR_64 base, u_int8 index)</arglist>
    </member>
  </compound>
  <compound kind="page">
    <name>iddummy</name>
    <title>MEN logo</title>
    <filename>iddummy</filename>
  </compound>
</tagfile>
