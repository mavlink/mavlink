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
  <h3> <!-- mavlink_message_name -->
   <xsl:attribute name="id"><xsl:value-of select="@name"/></xsl:attribute>
   <xsl:value-of select="@name" /> (
   <a>
    <xsl:attribute name="href">#<xsl:value-of select="@name"/></xsl:attribute>
    #<xsl:value-of select="@id" />
   </a>
  )</h3> 
   <p> <!-- description -->
     <xsl:if test='@id > 255'><strong>(MAVLink 2) </strong></xsl:if>
     <xsl:value-of select="description" /></p>
   <table class="sortable">
   <thead>
   <tr> <!-- mavlink_field_header -->
     <th>Field Name</th>
     <th>Type</th>

     <xsl:if test='*/@units'>
      <th>Units</th>
     </xsl:if>
     
     <xsl:if test='*/@enum'>
      <th>Values</th>
     </xsl:if>

     <th>Description</th>
   </tr>
   </thead>
   <tbody>
   <xsl:apply-templates select="field" /> 
  </tbody>
  </table>
</xsl:template>

<xsl:template match="//field">
   <tr> <!-- mavlink_field -->
   <xsl:choose>
     <xsl:when test="preceding-sibling::extensions">
       <td style="color:blue;"><xsl:value-of select="@name" />&#160;<a href="#mav2_extension_field" title="MAVLink2 extension field">**</a></td> <!-- mavlink_name -->
     </xsl:when>
     <xsl:otherwise>
       <td><xsl:value-of select="@name" /></td> <!-- mavlink_name -->
     </xsl:otherwise>
   </xsl:choose>
  
   <td><xsl:value-of select="@type" /></td> <!-- mavlink_type -->
   
   <xsl:if test='../*/@units'>
     <td><xsl:value-of select="@units" /></td> <!-- mavlink_units -->
   </xsl:if>
   
   <xsl:if test='../*/@enum'>
     <td> 
      <a><xsl:attribute name="href">#<xsl:value-of select="@enum" /></xsl:attribute><xsl:value-of select="@enum" /></a>
     </td> <!-- mavlink_value -->
   </xsl:if>
     
   <td> <xsl:value-of select="." /> </td> <!-- mavlink_comment -->
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
   <h3> <!-- mavlink_message_name -->
     <xsl:attribute name="id"><xsl:value-of select="@name"/></xsl:attribute>
     <a><xsl:attribute name="href">#<xsl:value-of select="@name"/></xsl:attribute>
     <xsl:value-of select="@name" /></a></h3>

   <p><xsl:value-of select="description" /></p> <!-- description -->
   <table class="sortable">
   <thead>
   <tr> <!-- mavlink_field_header -->
     <th>Value</th>
     <th>Field Name</th>
     <th>Description</th>
   </tr>
   </thead>
   <tbody>
   <xsl:apply-templates select="entry" />
  </tbody>
  </table>
</xsl:template>

<xsl:template match="//entry">
   <tr id="{@name}"> <!-- mavlink_field -->
   <td><xsl:value-of select="@value" /></td>  <!-- mavlink_type -->
   <td><a><xsl:attribute name="href">#<xsl:value-of select="@name"/></xsl:attribute>
   <xsl:value-of select="@name" /></a></td> <!-- mavlink_name -->
   <td><xsl:value-of select="description" /></td> <!-- mavlink_comment -->
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
   <td>Mission Param #<xsl:value-of select="@index" /></td> <!-- mission_param -->
   <td><xsl:value-of select="." /></td> <!-- mavlink_comment -->
   </tr>
</xsl:template>


</xsl:stylesheet>
