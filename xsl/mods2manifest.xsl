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

  <xsl:template match="/mods:mods/mods:name[1]/mods:namePart">
    <xsl:call-template name="metadata">
      <xsl:with-param name="label">Author/creator</xsl:with-param>
      <xsl:with-param name="firstvalue" select="."/>
      <xsl:with-param name="values" select="../../mods:name/mods:namePart[not(@type='date')]"/>
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
                <value>
                  <xsl:attribute name="value">
                    <xsl:value-of select="normalize-space($firstvalue/text())"/>
                  </xsl:attribute>
                  <xsl:attribute name="language">
                    <xsl:value-of select="@lang"/>
                  </xsl:attribute>
                </value>
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
