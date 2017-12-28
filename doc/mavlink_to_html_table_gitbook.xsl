<?xml version="1.0"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="//include">
   <p><strong>MAVLink Include Files: </strong> <a><xsl:attribute name="href"><xsl:value-of select="."/>.md.unlikely</xsl:attribute><xsl:value-of select="." /></a> </p>
</xsl:template>

<xsl:template match="//enums">
   <h2>MAVLink Type Enumerations</h2>
   <xsl:apply-templates />
</xsl:template>

<xsl:template match="//messages">
   <h2>MAVLink Messages</h2>
   <xsl:apply-templates />
</xsl:template>

<xsl:template match="//message">
  <h3 class="mavlink_message_name">
   <xsl:attribute name="id"><xsl:value-of select="@name"/></xsl:attribute>
   <xsl:value-of select="@name" /> (
   <a>
    <xsl:attribute name="href">#<xsl:value-of select="@name"/></xsl:attribute>
    #<xsl:value-of select="@id" />
   </a>
  )</h3>
   <p class="description">
     <xsl:if test='@id > 255'><strong>(MAVLink 2) </strong></xsl:if>
     <xsl:value-of select="description" /></p>
   <table class="sortable">
   <thead>
   <tr>
     <th class="mavlink_field_header">Field Name</th>
     <th class="mavlink_field_header">Type</th>
     <th class="mavlink_field_header">Description</th>
   </tr>
   </thead>
   <tbody>
   <xsl:apply-templates select="field" /> 
  </tbody>
  </table>
</xsl:template>

<xsl:template match="//field">
   <tr class="mavlink_field">
   <xsl:choose>
     <xsl:when test="preceding-sibling::extensions">
       <td class="mavlink_name" valign="top" style="color:blue;"><xsl:value-of select="@name" />&#160;<a href="#mav2_extension_field" title="MAVLink2 extension field">**</a></td>
     </xsl:when>
     <xsl:otherwise>
       <td class="mavlink_name" valign="top"><xsl:value-of select="@name" /></td>
     </xsl:otherwise>
   </xsl:choose>
  
   <td class="mavlink_type" valign="top"><xsl:value-of select="@type" /></td>
   <td class="mavlink_comment"> <xsl:value-of select="." />
     <xsl:if test='@units'>
     (Units: <xsl:value-of select="@units" />)
     </xsl:if>
     <xsl:if test='@enum'>
     (Enum: <a><xsl:attribute name="href">#<xsl:value-of select="@enum" /></xsl:attribute><xsl:value-of select="@enum" /></a>)
     </xsl:if>
   </td>
   </tr>
</xsl:template>

<xsl:template match="//version">
   <h2>MAVLink Protocol Version</h2>
   <p>The current MAVLink version is 2.<xsl:value-of select="." />. The minor version numbers (after the dot) range from 1-255. </p>
</xsl:template>

<xsl:template match="//dialect">
   <p>This file has protocol dialect: <xsl:value-of select="." />.</p>
</xsl:template>

<xsl:template match="//enum">
   <h3 class="mavlink_message_name">    
     <xsl:attribute name="id"><xsl:value-of select="@name"/></xsl:attribute>
     <a><xsl:attribute name="href">#<xsl:value-of select="@name"/></xsl:attribute>
     <xsl:value-of select="@name" /></a></h3>

   <p class="description"><xsl:value-of select="description" /></p>
   <table class="sortable">
   <thead>
   <tr>
     <th class="mavlink_field_header">CMD ID</th>
     <th class="mavlink_field_header">Field Name</th>
     <th class="mavlink_field_header">Description</th>
   </tr>
   </thead>
   <tbody>
   <xsl:apply-templates select="entry" />
  </tbody>
  </table>
</xsl:template>

<xsl:template match="//entry">
   <tr class="mavlink_field" id="{@name}">
   <td class="mavlink_type" valign="top"><xsl:value-of select="@value" /></td>
   <td class="mavlink_name" valign="top"><a><xsl:attribute name="href">#<xsl:value-of select="@name"/></xsl:attribute>
   <xsl:value-of select="@name" /></a></td>
   <td class="mavlink_comment"><xsl:value-of select="description" /></td>
   </tr>
<xsl:if test='param'>
   <tr>
     <td></td>
     <xsl:apply-templates select="param" />
   </tr>
   <tr>
    <td colspan="3"><br /></td>
   </tr>
</xsl:if>
   

</xsl:template>

<xsl:template match="//param">
   <tr>
   <td></td>
   <td class="mavlink_mission_param" valign="top">Mission Param #<xsl:value-of select="@index" /></td>
   <td class="mavlink_comment"><xsl:value-of select="." /></td>
   </tr>
</xsl:template>


</xsl:stylesheet>
