/**
 *
 */
SyntaxHighlighter.autoloader(
  'js jscript javascript  js/syntaxhighlighter/scripts/shBrushJScript.js',
  'php drupal             js/syntaxhighlighter/scripts/shBrushPhp.js'
);
SyntaxHighlighter.defaults['tab-size'] = 2;
SyntaxHighlighter.defaults['toolbar'] = false;
SyntaxHighlighter.all();