<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
	xmlns:xlink="http://www.w3.org/1999/xlink" 
	xmlns:mods="http://www.loc.gov/mods/v3"	
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	exclude-result-prefixes="mods xlink">
  <xsl:output method="xml" indent="yes" encoding="UTF-8"/>

  <xsl:template match="/">
    <manifest>
      <xsl:apply-templates/>
    </manifest>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:titleInfo/mods:title[1]">
    <xsl:call-template name="metadata">
      <xsl:with-param name="label">Title</xsl:with-param>
      <xsl:with-param name="firstvalue" select="."/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:titleInfo/mods:subTitle[1]">
    <xsl:call-template name="metadata">
      <xsl:with-param name="label">Subtitle</xsl:with-param>
      <xsl:with-param name="firstvalue" select="."/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:note[1]">
    <xsl:call-template name="metadata">
       <xsl:with-param name="label">Note</xsl:with-param>
       <xsl:with-param name="firstvalue" select="."/>
       <xsl:with-param name="values" select="../mods:note"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:accessCondition[@type='use and reproduction']">
    <license><xsl:value-of select="@xlink:href"/></license>
  </xsl:template>

  <xsl:template match="text()|@*">
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template name="metadata">
    <xsl:param name="label"/>
    <xsl:param name="firstvalue"/>
    <xsl:param name="values" select="/.."/>
    <xsl:if test="string-length(normalize-space(concat($firstvalue/text(), $values/text()))) != 0">
      <metadata>
        <label><xsl:value-of select="$label"/></label>
        <xsl:choose>
          <xsl:when test="count($values) &gt; 1">
            <xsl:for-each select="$values">
              <xsl:if test="normalize-space(text())">
                <xsl:choose>
                  <xsl:when test="@lang">
                    <value>
                      <xsl:attribute name="value">
                        <xsl:value-of select="normalize-space(text())"/>
                      </xsl:attribute>
                      <xsl:attribute name="language">
                        <xsl:value-of select="@lang"/>
                      </xsl:attribute>
                    </value>
                  </xsl:when>
                  <xsl:otherwise>
                    <value><xsl:value-of select="normalize-space(text())"/></value>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:if>
            </xsl:for-each>
          </xsl:when>
          <xsl:otherwise>
            <xsl:choose>
              <xsl:when test="@lang">
                <xsl:attribute name="value">
                  <xsl:value-of select="normalize-space($firstvalue/text())"/>
                </xsl:attribute>
                <xsl:attribute name="language">
                  <xsl:value-of select="@lang"/>
                </xsl:attribute>
              </xsl:when>
              <xsl:otherwise>
                 <value><xsl:value-of select="normalize-space($firstvalue/text())"/></value>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:otherwise>
        </xsl:choose>
      </metadata>
    </xsl:if> 
  </xsl:template>
</xsl:stylesheet>
