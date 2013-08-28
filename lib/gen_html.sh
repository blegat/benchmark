PROG=$1
echo "<!DOCTYPE html>
<html>
  <head>
    <meta charset=\"utf-8\" />
    <title>$PROG</title>
  </head>
  <body>
  <h1>Benchmark \"$PROG\"</h1>
  <h2>Introduction</h2>"
cat README.html
echo "</body>
<h2>Résultat du benchmark</h2>
<img src=\"$PROG.png\" alt=\"Résultats du benchmark de $PROG\" />
</html>"
