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
      <attribution>Leiden University Libraries</attribution>
    </manifest>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:titleInfo[not(@type='translated')]/mods:title[1]">
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

  <xsl:template match="/mods:mods/mods:originInfo[not(@eventType)]/mods:dateIssued[1]">
    <xsl:call-template name="metadata">
      <xsl:with-param name="label">Date issued</xsl:with-param>
      <xsl:with-param name="type">date</xsl:with-param>
      <xsl:with-param name="firstvalue" select="."/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:tableOfContents[1]">
    <xsl:call-template name="metadata">
      <xsl:with-param name="label">Content</xsl:with-param>
      <xsl:with-param name="firstvalue" select="."/>
      <xsl:with-param name="values" select="../mods:tableOfContents"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:titleInfo[@type='translated']/mods:title[1]">
    <xsl:call-template name="metadata">
      <xsl:with-param name="label">Translated title</xsl:with-param>
      <xsl:with-param name="firstvalue" select="."/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:name[1]/mods:namePart[not(@type)][1]">
    <xsl:call-template name="metadata">
      <xsl:with-param name="label">Author/creator</xsl:with-param>
      <xsl:with-param name="type">author</xsl:with-param>
      <xsl:with-param name="firstvalue" select=".."/>
      <xsl:with-param name="values" select="../../mods:name"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:relatedItem[@type='otherFormat']/mods:identifier[1]">
    <xsl:call-template name="metadata">
      <xsl:with-param name="label">Shelfmark</xsl:with-param>
      <xsl:with-param name="firstvalue" select="."/>
      <xsl:with-param name="values" select="../mods:identifier"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:relatedItem[@type='otherFormat']/mods:note[1]">
    <xsl:call-template name="metadata">
      <xsl:with-param name="label">Shelfmark (note)</xsl:with-param>
      <xsl:with-param name="firstvalue" select="."/>
      <xsl:with-param name="values" select="../mods:note"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:subject/mods:topic">
    <xsl:call-template name="metadata">
      <xsl:with-param name="label">Subject (topical)</xsl:with-param>
      <xsl:with-param name="firstvalue" select="."/>
      <xsl:with-param name="values" select="../mods:topic"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:subject/mods:geographic">
    <xsl:call-template name="metadata">
      <xsl:with-param name="label">Subject (geographic)</xsl:with-param>
      <xsl:with-param name="firstvalue" select="."/>
      <xsl:with-param name="values" select="../mods:geographic"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:subject/mods:temporal">
    <xsl:call-template name="metadata">
      <xsl:with-param name="label">Subject (temporal)</xsl:with-param>
      <xsl:with-param name="firstvalue" select="."/>
      <xsl:with-param name="values" select="../mods:temporal"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:note[not(@type='ownership')][1]">
    <xsl:call-template name="metadata">
      <xsl:with-param name="label">Note</xsl:with-param>
      <xsl:with-param name="firstvalue" select="."/>
      <xsl:with-param name="values" select="../mods:note[not(@type='ownership')]"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:abstract[@type='Scope and content'][1]">
    <xsl:call-template name="metadata">
      <xsl:with-param name="label">Note</xsl:with-param>
      <xsl:with-param name="firstvalue" select="."/>
      <xsl:with-param name="values" select="../mods:abstract[@type='Scope and content']"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:relatedItem[@type='isReferencedBy']/mods:titleInfo/mods:title[1]">
    <xsl:call-template name="metadata">
      <xsl:with-param name="label">Reference</xsl:with-param>
      <xsl:with-param name="firstvalue" select="."/>
      <xsl:with-param name="values" select="../mods:title"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:abstract[not(@type='Scope and content')][not(@type='Abstract')][1]">
    <xsl:call-template name="metadata">
      <xsl:with-param name="label">Abstract</xsl:with-param>
      <xsl:with-param name="firstvalue" select="."/>
      <xsl:with-param name="values" select="../mods:abstract[not(@type='Scope and content')]"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:language/mods:languageTerm[@type='text'][@authority='iso639-2b'][1]">
    <xsl:call-template name="metadata">
      <xsl:with-param name="label">Language</xsl:with-param>
      <xsl:with-param name="firstvalue" select="."/>
      <xsl:with-param name="values" select="../mods:languageTerm[@type='text'][@authority='iso639-2b']"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:originInfo/mods:place/mods:placeTerm[@type='text'][@authority='marccountry'][1]">
    <xsl:call-template name="metadata">
      <xsl:with-param name="label">Country</xsl:with-param>
      <xsl:with-param name="firstvalue" select="."/>
      <xsl:with-param name="values" select="../mods:placeTerm[@type='text'][@authority='marccountry']"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:relatedItem[@type='otherFormat']/mods:physicalDescription/mods:extent[1]">
    <xsl:call-template name="metadata">
      <xsl:with-param name="label">Form</xsl:with-param>
      <xsl:with-param name="firstvalue" select="."/>
      <xsl:with-param name="values" select="../mods:extent"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:relatedItem[@type='original'][1]/mods:titleInfo/mods:title[1]">
    <xsl:call-template name="metadata">
      <xsl:with-param name="label">Scanned from</xsl:with-param>
      <xsl:with-param name="firstvalue" select="."/>
      <xsl:with-param name="values" select="../../../mods:relatedItem[@type='original']/mods:titleInfo/mods:title"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:location/mods:physicalLocation">
    <xsl:call-template name="metadata">
      <xsl:with-param name="label">Location</xsl:with-param>
      <xsl:with-param name="firstvalue" select="."/>
      <xsl:with-param name="values" select="../mods:physicalLocation"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:note[@type='ownership'][1]">
    <xsl:call-template name="metadata">
      <xsl:with-param name="label">Ownership</xsl:with-param>
      <xsl:with-param name="firstvalue" select="."/>
      <xsl:with-param name="values" select="../mods:note[@type='ownership']"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:relatedItem[@type='otherFormat'][1]/mods:originInfo/mods:publisher[1]">
    <xsl:call-template name="metadata">
      <xsl:with-param name="label">Published</xsl:with-param>
      <xsl:with-param name="firstvalue" select="."/>
      <xsl:with-param name="values" select="../../../mods:relatedItem[@type='otherFormat']/mods:originInfo/mods:publisher"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:relatedItem[@type='host'][1]/mods:titleInfo/mods:title[1]">
    <xsl:call-template name="metadata">
      <xsl:with-param name="label">Part of</xsl:with-param>
      <xsl:with-param name="firstvalue" select="."/>
      <xsl:with-param name="values" select="../../../mods:relatedItem[@type='host']/mods:titleInfo/mods:title"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:relatedItem[@type='host'][1]/mods:identifier[@type='uri'][1]">
    <xsl:call-template name="metadata">
      <xsl:with-param name="label">Collection guide</xsl:with-param>
      <xsl:with-param name="firstvalue" select="."/>
      <xsl:with-param name="values" select="../../mods:relatedItem[@type='host']/mods:identifier[@type='uri']"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:identifier[@type='hdl'][1]">
    <xsl:call-template name="metadata">
      <xsl:with-param name="label">Persistent URL</xsl:with-param>
      <xsl:with-param name="firstvalue" select="."/>
      <xsl:with-param name="values" select="../mods:identifier[@type='hdl']"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:originInfo[1]/mods:publisher[1]">
    <xsl:call-template name="metadata">
      <xsl:with-param name="label">Published (digital)</xsl:with-param>
      <xsl:with-param name="firstvalue" select="."/>
      <xsl:with-param name="values" select="../../mods:originInfo/mods:publisher"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:originInfo[@eventType='publication'][1]/mods:dateIssued[1]">
    <xsl:call-template name="metadata">
      <xsl:with-param name="label">Publication date</xsl:with-param>
      <xsl:with-param name="type">date</xsl:with-param>
      <xsl:with-param name="firstvalue" select="."/>
      <xsl:with-param name="values" select="../../mods:originInfo[@eventType='publication']/mods:dateIssued"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:physicalDescription/mods:extent[1]">
    <xsl:call-template name="metadata">
      <xsl:with-param name="label">Extent</xsl:with-param>
      <xsl:with-param name="firstvalue" select="."/>
      <xsl:with-param name="values" select="../mods:extent"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:relatedItem[@type='otherVersion'][1]/mods:titleInfo/mods:title[1]">
    <xsl:call-template name="metadata">
      <xsl:with-param name="label">Part of</xsl:with-param>
      <xsl:with-param name="firstvalue" select="."/>
      <xsl:with-param name="values" select="../../../mods:relatedItem[@type='otherVersion']/mods:titleInfo/mods:title"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:abstract[@type='Abstract'][1]">
    <description>
      <xsl:for-each select="../mods:abstract[@type='Abstract']">
        <xsl:value-of select="normalize-space(text())"/>
        <xsl:value-of select="' '"/>
      </xsl:for-each>
    </description>
  </xsl:template>

  <xsl:template match="/mods:mods/mods:accessCondition[@type='restriction on access']">
    <license>
      <xsl:value-of select="text()"/>
      <xsl:text> &lt;a href=&quot;</xsl:text><xsl:value-of select="../mods:accessCondition[@type='use and reproduction']/@xlink:href"/><xsl:text>&quot;&gt;</xsl:text><xsl:value-of select="../mods:accessCondition[@type='use and reproduction']/text()"/><xsl:text>&lt;/a&gt;</xsl:text>
    </license>
  </xsl:template>

  <xsl:template match="text()|@*">
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template name="metadata">
    <xsl:param name="label"/>
    <xsl:param name="type" select="normal"/>
    <xsl:param name="firstvalue"/>
    <xsl:param name="values" select="/.."/>
    <xsl:if test="string-length(normalize-space(concat(string($firstvalue), string($values)))) != 0">
      <metadata>
        <label><xsl:value-of select="$label"/></label>
        <xsl:choose>
          <xsl:when test="count($values) &gt; 1">
            <xsl:for-each select="$values">
              <xsl:if test="normalize-space(string(.))">
                <xsl:choose>
                  <xsl:when test="@lang">
                    <value>
                      <xsl:attribute name="value">
                        <xsl:call-template name="mdvalue">
                          <xsl:with-param name="value" select="."/>
                          <xsl:with-param name="type" select="$type"/>
                        </xsl:call-template>
                      </xsl:attribute>
                      <xsl:attribute name="language">
                        <xsl:value-of select="@lang"/>
                      </xsl:attribute>
                    </value>
                  </xsl:when>
                  <xsl:otherwise>
                    <value>
                      <xsl:call-template name="mdvalue">
                        <xsl:with-param name="value" select="."/>
                        <xsl:with-param name="type" select="$type"/>
                      </xsl:call-template>
                    </value>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:if>
            </xsl:for-each>
          </xsl:when>
          <xsl:otherwise>
            <xsl:choose>
              <xsl:when test="@lang">
                <value>
                  <xsl:attribute name="value">
                    <xsl:call-template name="mdvalue">
                      <xsl:with-param name="value" select="$firstvalue"/>
                      <xsl:with-param name="type" select="$type"/>
                    </xsl:call-template>
                  </xsl:attribute>
                  <xsl:attribute name="language">
                    <xsl:value-of select="@lang"/>
                  </xsl:attribute>
                </value>
              </xsl:when>
              <xsl:otherwise>
                 <value>
                    <xsl:call-template name="mdvalue">
                      <xsl:with-param name="value" select="$firstvalue"/>
                      <xsl:with-param name="type" select="$type"/>
                    </xsl:call-template>
                 </value>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:otherwise>
        </xsl:choose>
      </metadata>
    </xsl:if>
  </xsl:template>

  <xsl:template name="mdvalue">
    <xsl:param name="value"/>
    <xsl:param name="type"/>

    <xsl:choose>
      <xsl:when test="$type = 'author'">
        <xsl:choose>
          <xsl:when test="string-length(normalize-space($value/mods:displayForm)) &gt; 0">
            <xsl:value-of select="normalize-space($value/mods:displayForm)"/>
          </xsl:when>
          <xsl:when test="string-length(concat(normalize-space($value/mods:namePart[@type='given']), normalize-space($value/mods:namePart[@type='family']))) &gt; 0">
            <xsl:if test="string-length(normalize-space($value/mods:namePart[@type='given'])) &gt; 0">
              <xsl:value-of select="concat(normalize-space($value/mods:namePart[@type='given']), ' ')"/>
            </xsl:if>
            <xsl:value-of select="normalize-space($value/mods:namePart[@type='family'])"/>
          </xsl:when>
          <xsl:when test="string-length(normalize-space($value/mods:namePart[not(@type)])) &gt; 0">
            <xsl:value-of select="normalize-space($value/mods:namePart[not(@type)])"/>
          </xsl:when>
        </xsl:choose>
        <xsl:if test="string-length(normalize-space($value/mods:namePart[@type = 'date'])) &gt; 0">
          <xsl:value-of select="concat(' (', normalize-space($value/mods:namePart[@type = 'date']), ')')"/>
        </xsl:if>
      </xsl:when>
      <xsl:when test="$type = 'date'">
        <xsl:value-of select="concat($value, '&#160;')"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="normalize-space($value)"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
</xsl:stylesheet>
