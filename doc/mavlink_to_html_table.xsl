<?xml version="1.0"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="/">
   <table border="1" cellspacing="5" cellpadding="5">
   <tr>
   <th colspan="3">Messages</th>
   </tr>
<xsl:for-each select="messages/message">
   <tr>
   <th colspan="2"><xsl:value-of select="@name" /></th>
   <th><xsl:value-of select="@id" /></th>
   </tr>
   <tr>
   <th>field</th>
   <th>data type</th>
   <th>description</th>
   </tr>
   <xsl:apply-templates />
</xsl:for-each>
  </table>
</xsl:template>

<xsl:template match="//field">
   <tr>
   <td align="center"><xsl:value-of select="@name" /></td>
   <td align="center"><xsl:value-of select="@type" /></td>
   <td align="center"><xsl:value-of select="//field" /></td>
   </tr>
</xsl:template>

</xsl:stylesheet>
