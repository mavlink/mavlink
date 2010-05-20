<?xml version="1.0"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="/">
   <table border="1" cellspacing="10" cellpadding="5" class="sortable">
<xsl:for-each select="messages/message">
   <tr>
   <th colspan="2" class="mavlink_message_name"><xsl:value-of select="@name" /></th>
   <th class="mavlink_message_id"><xsl:value-of select="@id" /></th>
   </tr>
   <tr>
   <th class="mavlink_field">field</th>
   <th class="mavlink_type">data type</th>
   <th class="mavlink_comment">description</th>
   </tr>
   <xsl:apply-templates />
</xsl:for-each>
  </table>
</xsl:template>

<xsl:template match="//field">
   <tr class="mavlink_field">
   <td class="mavlink_name"><xsl:value-of select="@name" /></td>
   <td class="mavlink_type"><xsl:value-of select="@type" /></td>
   <td class="mavlink_comment"><xsl:value-of select="//field" /></td>
   </tr>
</xsl:template>

</xsl:stylesheet>
