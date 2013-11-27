<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:output method="xml" encoding="UTF-8"/>

	<xsl:variable name="ExtendedLog" select="'false'"/>

	<xsl:template name="getFileName">
		<xsl:param name="path"/>
		<xsl:choose>
			<xsl:when test="contains( $path, '/' )">
				<xsl:call-template name="getFileName">
					<xsl:with-param name="path" select="substring-after( $path, '/' )"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="contains( $path, '\' )">
				<xsl:call-template name="getFileName">
					<xsl:with-param name="path" select="substring-after( $path, '\' )"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$path"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="getFileExtension">
		<xsl:param name="path"/>
		<xsl:param name="recursive" select="'false'"/>

		<xsl:variable name="fileName">
			<xsl:choose>
				<xsl:when test="$recursive = 'true'">
					<xsl:call-template name="getFileName">
						<xsl:with-param name="path" select="$path"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="$path"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:choose>
			<xsl:when test="contains( $fileName, '.' )">
				<xsl:call-template name="getFileExtension">
					<xsl:with-param name="path" select="substring-after( $fileName, '.' )"/>
					<xsl:with-param name="recursive" select="'true'"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="$recursive = 'true'">
				<xsl:value-of select="$fileName"/>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<!--Template returns  Context: img tag-->
	<xsl:template name="getImagePath">
		<xsl:param name="src"/>

		<xsl:variable name="fileExtension">
			<xsl:call-template name="getFileExtension">
				<xsl:with-param name="path" select="$src"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:choose>
			<xsl:when test="$fileExtension = ''">
				<xsl:value-of select="concat('media/image', count( preceding::img[ @src ] ) + 1, '.png' )"/>
			</xsl:when>
			<xsl:when test="not($fileExtension = 'gif' and $fileExtension = 'jpeg' and $fileExtension = 'jpg' and $fileExtension = 'png')">
				<xsl:value-of select="concat('media/image', count( preceding::img[ @src ] ) + 1, '.png')"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="concat( 'media/image', count( preceding::img[ @src ] ) + 1, '.', $fileExtension )"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="getRefLevel">
		<xsl:param name="node" select="."/>
		<xsl:choose>
			<xsl:when test="$node/ancestor::a[ @href ] | $node/ancestor::img[ @src ]">
				<xsl:variable name="val">
					<xsl:call-template name="getRefLevel">
						<xsl:with-param name="node" select ="$node/parent"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:value-of select="$val + 1"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>0</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="getRefID">
		<xsl:variable name="cnt">
			<xsl:value-of select="count( preceding::a[ @href ] | preceding::img[ @src ] )"/>
		</xsl:variable>
		<xsl:variable name="lvl">
			<xsl:call-template name="getRefLevel"/>
		</xsl:variable>
		<xsl:value-of select="concat( 'rId', ( $cnt + $lvl + 8 ))"/>
	</xsl:template>

  <xsl:template name="getDocPrID">
    <!-- Пока сделаем так, чтобы идентефикаторы не совпадали -->
    <xsl:value-of select="count( preceding::img[ @src ] )"/>
  </xsl:template>

  <xsl:template name="left-trim">
		<xsl:param name="s" />
		<xsl:choose>
			<xsl:when test="substring($s, 1, 1) = ''">
				<xsl:value-of select="$s"/>
			</xsl:when>
			<xsl:when test="normalize-space(substring($s, 1, 1)) = ''">
				<xsl:call-template name="left-trim">
					<xsl:with-param name="s" select="substring($s, 2)" />
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$s" />
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="right-trim">
		<xsl:param name="s" />
		<xsl:choose>
			<xsl:when test="substring($s, 1, 1) = ''">
				<xsl:value-of select="$s"/>
			</xsl:when>
			<xsl:when test="normalize-space(substring($s, string-length($s))) = ''">
				<xsl:call-template name="right-trim">
					<xsl:with-param name="s" select="substring($s, 1, string-length($s) - 1)" />
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$s" />
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="trim">
		<xsl:param name="s" />
		<xsl:call-template name="right-trim">
			<xsl:with-param name="s">
				<xsl:call-template name="left-trim">
					<xsl:with-param name="s" select="$s" />
				</xsl:call-template>
			</xsl:with-param>
		</xsl:call-template>
	</xsl:template>

	<xsl:template name="ConvertColor">
		<xsl:param name="color"/>

		<xsl:variable name="c">
			<xsl:call-template name="trim">
				<xsl:with-param name="s" select="$color"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:variable name="rgb">
			<xsl:if test="contains($color, 'rgb')">
				<xsl:variable name="red">
					<xsl:value-of select="substring-before(substring-after($color, '('), ',')"/>
				</xsl:variable>
				<xsl:variable name="green">
					<xsl:value-of select="substring-before(substring-after($color, concat('(', $red, ',')), ',')"/>
				</xsl:variable>
				<xsl:variable name="blue">
					<xsl:value-of select="substring-before(substring-after($color, concat('(', $red, ',', $green, ',')), ')')"/>
				</xsl:variable>
				<xsl:call-template name="toHex">
					<xsl:with-param name="dec">
						<xsl:value-of select="$red"/>
					</xsl:with-param>
				</xsl:call-template>
				<xsl:call-template name="toHex">
					<xsl:with-param name="dec">
						<xsl:value-of select="$green"/>
					</xsl:with-param>
				</xsl:call-template>
				<xsl:call-template name="toHex">
					<xsl:with-param name="dec">
						<xsl:value-of select="$blue"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:if>
		</xsl:variable>

		<xsl:choose>
			<xsl:when test="starts-with( $c, '#' )">
				<xsl:variable name="col">
					<xsl:value-of select="substring-after( $color, '#' )"/>
				</xsl:variable>
				<xsl:choose>
					<xsl:when test="string-length($col) = 6">
						<xsl:value-of select="$col"/>
					</xsl:when>
					<xsl:when test="string-length($col) = 3">
						<xsl:value-of select="substring($col, 1, 1)"/>
						<xsl:value-of select="substring($col, 1, 1)"/>
						<xsl:value-of select="substring($col, 2, 1)"/>
						<xsl:value-of select="substring($col, 2, 1)"/>
						<xsl:value-of select="substring($col, 3, 1)"/>
						<xsl:value-of select="substring($col, 3, 1)"/>
					</xsl:when>
				</xsl:choose>
			</xsl:when>
			<xsl:when test="$c = 'inherit'">
				<!--STUB-->
				<xsl:text>FFFFFF</xsl:text>
			</xsl:when>
			<xsl:when test="$c = 'transparent'">
				<!--STUB-->
				<xsl:text>FFFFFF</xsl:text>
			</xsl:when>
			<xsl:when test="$rgb != ''">
				<xsl:value-of select="$rgb"/>
			</xsl:when>
			<xsl:when test="contains('aliceblue antiquewhite blanchedalmond blueviolet burlywood cadetblue chartreuse chocolate cornflowerblue cornsilk crimson darkblue darkcyan darkgoldenrod darkgray darkgreen darkkhaki darkmagenta darkolivegreen darkorange darkorchid darkred darksalmon darkseagreen darkslateblue darkslategray darkturquoise darkviolet deeppink deepskyblue dimgray dodgerblue firebrick floralwhite forestgreen fuchsia gainsboro ghostwhite greenyellow honeydew hotpink indianred lavenderblush lawngreen lemonchiffon lightblue lightcoral lightcyan lightgoldenrodyellow lightgray lightgreen lightpink lightsalmon lightseagreen lightskyblue lightslategray lightsteelblue lightyellow limegreen mediumaquamarine mediumblue mediumorchid mediumpurple mediumseagreen mediumslateblue mediumspringgreen mediumturquoise mediumvioletred midnightblue mintcream mistyrose moccasin navajowhite oldlace olivedrab orangered palegoldenrod palegreen paleturquoise palevioletred papayawhip peachpuff powderblue rosybrown royalblue saddlebrown sandybrown seashell skyblue slategray steelblue whitesmoke yellowgreen aquamarine goldenrod seagreen slateblue springgreen aqua azure beige bisque black blue brown coral cyan gold gray green indigo ivory khaki lavender lime linen magenta maroon navy olive orange orchid peru pink plum purple red salmon sienna silver snow tan teal thistle tomato turquoise violet wheat white yellowblack blue cyan green magenta red yellow', translate($c, 'ABCDEFGHIJKLMNOPQRSTUVWXYZ', 'abcdefghijklmnopqrstuvwxyz'))">
				<xsl:call-template name="NormalizeColorName">
					<xsl:with-param name="colorName" select="$c"/>
				</xsl:call-template>
				<!--<xsl:value-of select="$c"/>-->
			</xsl:when>
			<!--<xsl:otherwise>
              <xsl:value-of select="$c"/>
            </xsl:otherwise>-->
		</xsl:choose>
	</xsl:template>

	<xsl:template name="NormalizeColorName">
		<xsl:param name = "ColorName"/>

		<xsl:choose>

      <xsl:when test="$ColorName='aliceblue'">           <xsl:text>#F0F8FF</xsl:text></xsl:when>
      <xsl:when test="$ColorName='antiquewhite'">        <xsl:text>#FAEBD7</xsl:text></xsl:when>
      <xsl:when test="$ColorName='blanchedalmond'">      <xsl:text>#FFEBCD</xsl:text></xsl:when>
      <xsl:when test="$ColorName='blueviolet'">          <xsl:text>#8A2BE2</xsl:text></xsl:when>
      <xsl:when test="$ColorName='burlywood'">           <xsl:text>#DEB887</xsl:text></xsl:when>
      <xsl:when test="$ColorName='cadetblue'">           <xsl:text>#5F9EA0</xsl:text></xsl:when>
      <xsl:when test="$ColorName='chartreuse'">          <xsl:text>#7FFF00</xsl:text></xsl:when>
      <xsl:when test="$ColorName='chocolate'">           <xsl:text>#D2691E</xsl:text></xsl:when>
      <xsl:when test="$ColorName='cornflowerblue'">      <xsl:text>#6495ED</xsl:text></xsl:when>
      <xsl:when test="$ColorName='cornsilk'">            <xsl:text>#FFF8DC</xsl:text></xsl:when>
      <xsl:when test="$ColorName='crimson'">             <xsl:text>#DC143C</xsl:text></xsl:when>
      <xsl:when test="$ColorName='darkblue'">            <xsl:text>#00008B</xsl:text></xsl:when>
      <xsl:when test="$ColorName='darkcyan'">            <xsl:text>#008B8B</xsl:text></xsl:when>
      <xsl:when test="$ColorName='darkgoldenrod'">       <xsl:text>#B8860B</xsl:text></xsl:when>
      <xsl:when test="$ColorName='darkgray'">            <xsl:text>#A9A9A9</xsl:text></xsl:when>
      <xsl:when test="$ColorName='darkgreen'">           <xsl:text>#006400</xsl:text></xsl:when>
      <xsl:when test="$ColorName='darkkhaki'">           <xsl:text>#BDB76B</xsl:text></xsl:when>
      <xsl:when test="$ColorName='darkmagenta'">         <xsl:text>#8B008B</xsl:text></xsl:when>

      <xsl:when test="$ColorName='darkolivegreen'">      <xsl:text>#556B2F</xsl:text></xsl:when>
      <xsl:when test="$ColorName='darkorange'">          <xsl:text>#FF8C00</xsl:text></xsl:when>
      <xsl:when test="$ColorName='darkorchid'">          <xsl:text>#9932CC</xsl:text></xsl:when>
      <xsl:when test="$ColorName='darkred'">             <xsl:text>#8B0000</xsl:text></xsl:when>
      <xsl:when test="$ColorName='darksalmon'">          <xsl:text>#E9967A</xsl:text></xsl:when>
      <xsl:when test="$ColorName='darkseagreen'">        <xsl:text>#8FBC8F</xsl:text></xsl:when>
      <xsl:when test="$ColorName='darkslateblue'">       <xsl:text>#483D8B</xsl:text></xsl:when>
      <xsl:when test="$ColorName='darkslategray'">       <xsl:text>#2F4F4F</xsl:text></xsl:when>
      <xsl:when test="$ColorName='darkturquoise'">       <xsl:text>#00CED1</xsl:text></xsl:when>
      <xsl:when test="$ColorName='darkviolet'">          <xsl:text>#9400D3</xsl:text></xsl:when>
      <xsl:when test="$ColorName='deeppink'">            <xsl:text>#FF1493</xsl:text></xsl:when>
      <xsl:when test="$ColorName='deepskyblue'">         <xsl:text>#00BFFF</xsl:text></xsl:when>
      <xsl:when test="$ColorName='dimgray'">             <xsl:text>#696969</xsl:text></xsl:when>
      <xsl:when test="$ColorName='dodgerblue'">          <xsl:text>#1E90FF</xsl:text></xsl:when>
      <xsl:when test="$ColorName='firebrick'">           <xsl:text>#B22222</xsl:text></xsl:when>
      <xsl:when test="$ColorName='floralwhite'">         <xsl:text>#FFFAF0</xsl:text></xsl:when>
      <xsl:when test="$ColorName='forestgreen'">         <xsl:text>#228B22</xsl:text></xsl:when>
      <xsl:when test="$ColorName='fuchsia'">             <xsl:text>#FF00FF</xsl:text></xsl:when>
      <xsl:when test="$ColorName='gainsboro'">           <xsl:text>#DCDCDC</xsl:text></xsl:when>
      <xsl:when test="$ColorName='ghostwhite'">          <xsl:text>#F8F8FF</xsl:text></xsl:when>
      <xsl:when test="$ColorName='greenyellow'">         <xsl:text>#ADFF2F</xsl:text></xsl:when>
      <xsl:when test="$ColorName='honeydew'">            <xsl:text>#F0FFF0</xsl:text></xsl:when>
      <xsl:when test="$ColorName='hotpink'">             <xsl:text>#FF69B4</xsl:text></xsl:when>
      <xsl:when test="$ColorName='indianred'">           <xsl:text>#CD5C5C</xsl:text></xsl:when>

      <xsl:when test="$ColorName='lavenderblush'">       <xsl:text>#FFF0F5</xsl:text></xsl:when>
      <xsl:when test="$ColorName='lawngreen'">           <xsl:text>#7CFC00</xsl:text></xsl:when>
      <xsl:when test="$ColorName='lemonchiffon'">        <xsl:text>#FFFACD</xsl:text></xsl:when>
      <xsl:when test="$ColorName='lightblue'">           <xsl:text>#ADD8E6</xsl:text></xsl:when>
      <xsl:when test="$ColorName='lightcoral'">          <xsl:text>#F08080</xsl:text></xsl:when>
      <xsl:when test="$ColorName='lightcyan'">           <xsl:text>#E0FFFF</xsl:text></xsl:when>
      <xsl:when test="$ColorName='lightgoldenrodyellow'"><xsl:text>#FAFAD2</xsl:text></xsl:when>
      <xsl:when test="$ColorName='lightgray'">           <xsl:text>#D3D3D3</xsl:text></xsl:when>
      <xsl:when test="$ColorName='lightgreen'">          <xsl:text>#90EE90</xsl:text></xsl:when>
      <xsl:when test="$ColorName='lightpink'">           <xsl:text>#FFB6C1</xsl:text></xsl:when>
      <xsl:when test="$ColorName='lightsalmon'">         <xsl:text>#FFA07A</xsl:text></xsl:when>
      <xsl:when test="$ColorName='lightseagreen'">       <xsl:text>#20B2AA</xsl:text></xsl:when>
      <xsl:when test="$ColorName='lightskyblue'">        <xsl:text>#87CEFA</xsl:text></xsl:when>
      <xsl:when test="$ColorName='lightslategray'">      <xsl:text>#778899</xsl:text></xsl:when>
      <xsl:when test="$ColorName='lightsteelblue'">      <xsl:text>#B0C4DE</xsl:text></xsl:when>
      <xsl:when test="$ColorName='lightyellow'">         <xsl:text>#FFFFE0</xsl:text></xsl:when>
      <xsl:when test="$ColorName='limegreen'">           <xsl:text>#32CD32</xsl:text></xsl:when>
      <xsl:when test="$ColorName='mediumaquamarine'">    <xsl:text>#66CDAA</xsl:text></xsl:when>
      <xsl:when test="$ColorName='mediumblue'">          <xsl:text>#0000CD</xsl:text></xsl:when>
      <xsl:when test="$ColorName='mediumorchid'">        <xsl:text>#BA55D3</xsl:text></xsl:when>

      <xsl:when test="$ColorName='mediumpurple'">        <xsl:text>#9370D8</xsl:text></xsl:when>
      <xsl:when test="$ColorName='mediumseagreen'">      <xsl:text>#3CB371</xsl:text></xsl:when>
      <xsl:when test="$ColorName='mediumslateblue'">     <xsl:text>#7B68EE</xsl:text></xsl:when>
      <xsl:when test="$ColorName='mediumspringgreen'">   <xsl:text>#00FA9A</xsl:text></xsl:when>
      <xsl:when test="$ColorName='mediumturquoise'">     <xsl:text>#48D1CC</xsl:text></xsl:when>
      <xsl:when test="$ColorName='mediumvioletred'">     <xsl:text>#C71585</xsl:text></xsl:when>
      <xsl:when test="$ColorName='midnightblue'">        <xsl:text>#191970</xsl:text></xsl:when>
      <xsl:when test="$ColorName='mintcream'">           <xsl:text>#F5FFFA</xsl:text></xsl:when>
      <xsl:when test="$ColorName='mistyrose'">           <xsl:text>#FFE4E1</xsl:text></xsl:when>
      <xsl:when test="$ColorName='moccasin'">            <xsl:text>#BA55D3</xsl:text></xsl:when>
      <xsl:when test="$ColorName='navajowhite'">         <xsl:text>#FFDEAD</xsl:text></xsl:when>
      <xsl:when test="$ColorName='oldlace'">             <xsl:text>#FDF5E6</xsl:text></xsl:when>
      <xsl:when test="$ColorName='olivedrab'">           <xsl:text>#688E23</xsl:text></xsl:when>
      <xsl:when test="$ColorName='orangered'">           <xsl:text>#FF4500</xsl:text></xsl:when>
      <xsl:when test="$ColorName='palegoldenrod'">       <xsl:text>#EEE8AA</xsl:text></xsl:when>
      <xsl:when test="$ColorName='palegreen'">           <xsl:text>#98FB98</xsl:text></xsl:when>
      <xsl:when test="$ColorName='paleturquoise'">       <xsl:text>#AFEEEE</xsl:text></xsl:when>
      <xsl:when test="$ColorName='palevioletred'">       <xsl:text>#D87093</xsl:text></xsl:when>
      <xsl:when test="$ColorName='papayawhip'">          <xsl:text>#FFEFD5</xsl:text></xsl:when>
      <xsl:when test="$ColorName='peachpuff'">           <xsl:text>#FFDAB9</xsl:text></xsl:when>
      <xsl:when test="$ColorName='powderblue'">          <xsl:text>#B0E0E6</xsl:text></xsl:when>

      <xsl:when test="$ColorName='rosybrown'">           <xsl:text>#BC8F8F</xsl:text></xsl:when>
      <xsl:when test="$ColorName='royalblue'">           <xsl:text>#4169E1</xsl:text></xsl:when>
      <xsl:when test="$ColorName='saddlebrown'">         <xsl:text>#8B4513</xsl:text></xsl:when>
      <xsl:when test="$ColorName='sandybrown'">          <xsl:text>#F4A460</xsl:text></xsl:when>
      <xsl:when test="$ColorName='seashell'">            <xsl:text>#FFF5EE</xsl:text></xsl:when>
      <xsl:when test="$ColorName='skyblue'">             <xsl:text>#87CEEB</xsl:text></xsl:when>
      <xsl:when test="$ColorName='slategray'">           <xsl:text>#708090</xsl:text></xsl:when>
      <xsl:when test="$ColorName='steelblue'">           <xsl:text>#4682B4</xsl:text></xsl:when>
      <xsl:when test="$ColorName='whitesmoke'">          <xsl:text>#F5F5F5</xsl:text></xsl:when>
      <xsl:when test="$ColorName='yellowgreen'">         <xsl:text>#9ACD32</xsl:text></xsl:when>

      <xsl:when test="$ColorName='aquamarine'">          <xsl:text>#7FFFD4</xsl:text></xsl:when>
      <xsl:when test="$ColorName='goldenrod'">           <xsl:text>#DAA520</xsl:text></xsl:when>
      <xsl:when test="$ColorName='seagreen'">            <xsl:text>#2E8B57</xsl:text></xsl:when>
      <xsl:when test="$ColorName='slateblue'">           <xsl:text>#6A5ACD</xsl:text></xsl:when>
      <xsl:when test="$ColorName='springgreen'">         <xsl:text>#00FF7F</xsl:text></xsl:when>

      <xsl:when test="$ColorName='aqua'">                <xsl:text>#00FFFF</xsl:text></xsl:when>
      <xsl:when test="$ColorName='azure'">               <xsl:text>#F0FFFF</xsl:text></xsl:when>
      <xsl:when test="$ColorName='beige'">               <xsl:text>#F5F5DC</xsl:text></xsl:when>
      <xsl:when test="$ColorName='bisque'">              <xsl:text>#FFE4C4</xsl:text></xsl:when>
      <xsl:when test="$ColorName='black'">               <xsl:text>#000000</xsl:text></xsl:when>
      <xsl:when test="$ColorName='blue'">                <xsl:text>#0000FF</xsl:text></xsl:when>
      <xsl:when test="$ColorName='brown'">               <xsl:text>#A52A2A</xsl:text></xsl:when>
      <xsl:when test="$ColorName='coral'">               <xsl:text>#FF7F50</xsl:text></xsl:when>
      <xsl:when test="$ColorName='cyan'">                <xsl:text>#00FFFF</xsl:text></xsl:when>
      <xsl:when test="$ColorName='gold'">                <xsl:text>#FFD700</xsl:text></xsl:when>
      <xsl:when test="$ColorName='gray'">                <xsl:text>#808080</xsl:text></xsl:when>
      <xsl:when test="$ColorName='green'">               <xsl:text>#008000</xsl:text></xsl:when>
      <xsl:when test="$ColorName='indigo'">              <xsl:text>#4B0082</xsl:text></xsl:when>
      <xsl:when test="$ColorName='ivory'">               <xsl:text>#FFFFF0</xsl:text></xsl:when>
      <xsl:when test="$ColorName='khaki'">               <xsl:text>#F0E68C</xsl:text></xsl:when>
      <xsl:when test="$ColorName='lavender'">            <xsl:text>#E6E6FA</xsl:text></xsl:when>
      <xsl:when test="$ColorName='lime'">                <xsl:text>#00FF00</xsl:text></xsl:when>
      <xsl:when test="$ColorName='linen'">               <xsl:text>#FAF0E6</xsl:text></xsl:when>
      <xsl:when test="$ColorName='magenta'">             <xsl:text>#FF00FF</xsl:text></xsl:when>
      <xsl:when test="$ColorName='maroon'">              <xsl:text>#800000</xsl:text></xsl:when>
      <xsl:when test="$ColorName='navy'">                <xsl:text>#000080</xsl:text></xsl:when>
      <xsl:when test="$ColorName='olive'">               <xsl:text>#808000</xsl:text></xsl:when>
      <xsl:when test="$ColorName='orange'">              <xsl:text>#FFA500</xsl:text></xsl:when>
      <xsl:when test="$ColorName='orchid'">              <xsl:text>#DA70D6</xsl:text></xsl:when>
      <xsl:when test="$ColorName='peru'">                <xsl:text>#CD853F</xsl:text></xsl:when>
      <xsl:when test="$ColorName='pink'">                <xsl:text>#FFC0CB</xsl:text></xsl:when>
      <xsl:when test="$ColorName='plum'">                <xsl:text>#DDA0DD</xsl:text></xsl:when>
      <xsl:when test="$ColorName='purple'">              <xsl:text>#800080</xsl:text></xsl:when>
      <xsl:when test="$ColorName='red'">                 <xsl:text>#FF0000</xsl:text></xsl:when>
      <xsl:when test="$ColorName='salmon'">              <xsl:text>#FA8072</xsl:text></xsl:when>
      <xsl:when test="$ColorName='sienna'">              <xsl:text>#A0522D</xsl:text></xsl:when>
      <xsl:when test="$ColorName='silver'">              <xsl:text>#C0C0C0</xsl:text></xsl:when>
      <xsl:when test="$ColorName='snow'">                <xsl:text>#FFFAFA</xsl:text></xsl:when>
      <xsl:when test="$ColorName='tan'">                 <xsl:text>#D2B48C</xsl:text></xsl:when>
      <xsl:when test="$ColorName='teal'">                <xsl:text>#008080</xsl:text></xsl:when>
      <xsl:when test="$ColorName='thistle'">             <xsl:text>#D8BFD8</xsl:text></xsl:when>
      <xsl:when test="$ColorName='tomato'">              <xsl:text>#FF6347</xsl:text></xsl:when>
      <xsl:when test="$ColorName='turquoise'">           <xsl:text>#40E0D0</xsl:text></xsl:when>
      <xsl:when test="$ColorName='violet'">              <xsl:text>#EE82EE</xsl:text></xsl:when>
      <xsl:when test="$ColorName='wheat'">               <xsl:text>#F5DEB3</xsl:text></xsl:when>
      <xsl:when test="$ColorName='white'">               <xsl:text>#FFFFFF</xsl:text></xsl:when>
      <xsl:when test="$ColorName='yellow'">              <xsl:text>#FFFF00</xsl:text></xsl:when>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="toHex">
		<xsl:param name="dec"/>

		<xsl:variable name="first">
			<xsl:value-of select="(number($dec) - number($dec) mod 16) div 16"/>
		</xsl:variable>
		<xsl:variable name="second">
			<xsl:value-of select="(number($dec) mod 16)"/>
		</xsl:variable>
		<xsl:call-template name="hexTable">
			<xsl:with-param name="dec" select="$first"/>
		</xsl:call-template>
		<xsl:call-template name="hexTable">
			<xsl:with-param name="dec" select="$second"/>
		</xsl:call-template>
	</xsl:template>

	<xsl:template name="hexTable">
		<xsl:param name="dec"/>

		<xsl:variable name="decimal">
			<xsl:text>10</xsl:text>
		</xsl:variable>

		<xsl:choose>
			<xsl:when test="number($dec)&lt; 10">
				<xsl:value-of select="$dec"/>
			</xsl:when>
			<xsl:when test="$dec = '10'">
				<xsl:value-of select="'a'"/>
			</xsl:when>
			<xsl:when test="$dec = '11'">
				<xsl:value-of select="'b'"/>
			</xsl:when>
			<xsl:when test="$dec = '12'">
				<xsl:value-of select="'c'"/>
			</xsl:when>
			<xsl:when test="$dec = '13'">
				<xsl:value-of select="'d'"/>
			</xsl:when>
			<xsl:when test="$dec = '14'">
				<xsl:value-of select="'e'"/>
			</xsl:when>
			<xsl:when test="$dec = '15'">
				<xsl:value-of select="'f'"/>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<!--Get List Level-->
	<xsl:template name="getListLevel">
		<xsl:param name="node" select="."/>
		<xsl:value-of select="count(ancestor::ol | ancestor::ul) - 1"/>
	</xsl:template>


	<!--Get List NumId-->
	<xsl:template name="getListNumId">
		<xsl:param name="node" select="."/>
		<xsl:value-of select="count(preceding::ol[count(ancestor::ol) + count(ancestor::ul) = 0]) + count(preceding::ul[count(ancestor::ol) + count(ancestor::ul) = 0]) + 1"/>
	</xsl:template>


	<!--href validation-->
	<xsl:template name="hrefIsValid">
		<xsl:param name="href"/>

		<xsl:variable name="lowerHref">
			<xsl:value-of select="translate($href, 'ABCDEFGHIJKLMNOPQRSTUVWXYZ', 'abcdefghijklmnopqrstuvwxyz')"/>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="$lowerHref = ''">
				<xsl:text>false</xsl:text>
			</xsl:when>
			<xsl:when test="contains($lowerHref, 'http:')">
				<xsl:variable name="adress">
					<xsl:value-of select="substring-after($lowerHref, 'http:')"/>
				</xsl:variable>
				<xsl:variable name="adressWithoutSlashes">
					<xsl:call-template name="delSlashes">
						<xsl:with-param name="adress" select="$adress"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:choose>
					<xsl:when test="string-length($adressWithoutSlashes) = 0">
						<xsl:text>false</xsl:text>
					</xsl:when>
					<xsl:otherwise>
						<xsl:text>true</xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:when test="starts-with( $lowerHref, '#' )">
				<xsl:text>true</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>true</xsl:text>
			</xsl:otherwise>
		</xsl:choose>

	</xsl:template>


	<xsl:template name="delSlashes">
		<xsl:param name="adress"/>

		<xsl:choose>
			<xsl:when test="contains($adress, '\')">
				<xsl:call-template name="delSlashes">
					<xsl:with-param name="adress" select="concat(substring-before($adress, '\'), substring-after($adress, '\'))"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="contains($adress, '/')">
				<xsl:call-template name="delSlashes">
					<xsl:with-param name="adress" select="concat(substring-before($adress, '/'), substring-after($adress, '/'))"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$adress"/>
			</xsl:otherwise>
		</xsl:choose>

	</xsl:template>

</xsl:stylesheet>
