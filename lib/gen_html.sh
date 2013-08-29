prog=$1
echo "<!DOCTYPE html>
<html>
  <head>
    <meta charset=\"utf-8\" />
    <title>$prog</title>
  </head>
  <body>
  <h1>Benchmark \"$prog\"</h1>
  <h2>Introduction</h2>"
cat README.html
echo "</body>
<h2>Résultat du benchmark</h2>
<img src=\"$prog.png\" alt=\"Résultats du benchmark de $prog\" />"
shift
if [ $# -gt 0 ]; then
  echo "<h2>Perf results</h2>"
  if [ -e "perf_intro.html" ]; then
    echo "<h3>Introduction</h3>"
    cat perf_intro.html
  fi
  echo "<h3>Résultats</h3>"
  while [ $# -gt 0 ]; do
    filename="$1"
    name="${filename%.*}"
    echo "<h4>$name</h4>"
    if [ -e "$name.html" ]; then
      cat "$name.html"
    fi
    echo "<p><pre>"
    cat "$filename"
    echo "</pre></p>"
    shift
  done
fi
echo "</html>"
