<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<%
import static com.dexvis.util.GroovyUtil.renderTemplate as render;

println render("web/include/styles_multiples.inc", [:])

if (context.styles) {
  context.styles.each {
    style ->

    println "<style>";
    println new File(style).text;
    println "</style>";
  }
}

if (context.scripts) {
  context.scripts.each {
    script ->

    println "<script>"
    println new File(script).text;
    println "</script>"
  }
}
println render("web/include/scripts_multiples.inc", [:])
if (context.postscripts) {
  context.postscripts.each {
    script ->

    println "<script>"
    println new File(script).text;
    println "</script>"
  }
}
%>
  <title>${context.title}</title>
</head>
<body>
  <div class='layout-container'>
    <div id="basechart" style="display: none;"></div>
    <div class='ui-layout-west'>
      <div id='ConfigurationPane'></div>
    </div>
    <div class='ui-layout-center'>
      <div id='ChartArea'></div>
    </div>
  </div>

<!-- Enable Firebug 
<script type="text/javascript"
src="https://getfirebug.com/firebug-lite.js">
{
    overrideConsole: true,
    startInNewWindow: false,
    startOpened: true,
    enableTrace: true
}
</script>
-->

<script>

<% println dexData.toCsvString(); %>

var base = ${context.base}({ 'parent': '#basechart' });
base.load();

var multiples = dex.charts.multiples.Multiples({
    'parent': '#ChartArea',
    'csv': csv,
    'baseChart': base
  }); 

function save()
{
  multiples.save();
  base.save();
}

function setValues(values)
{
  values.forEach(function(config) {
    switch (config.chartName) {
      case "multiples":
      {
        multiples.attr(config.key, config.value);
        break;
      }
      case "basechart":
      {
        base.attrSave(config.key, config.value);
        break;
      }
    }
  });
  save();
}

function getGuiDefinition()
{
  var guiDef = {
    'multiples': multiples.getGuiDefinition(),
    'basechart': base.getGuiDefinition()
  };

  return JSON.stringify(guiDef);
}

\$(document).ready(function () {

  \$('.layout-container').layout({
    applyDemoStyles: false,
    west: {
      size: 335
    },
    onresize: function () {
       multiples.render();
    }
  });

  multiples.load();
  multiples.render();

  configPane = dex.ui.ConfigurationPane({
    "parent": "#ConfigurationPane",
    "csv": csv,
    "components": [ multiples, base ]
  }).render();
  
  multiples.subscribe(base, "attr", function() { multiples.render(); });
});
</script>
</body>
</html>