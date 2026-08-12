static const char* htmlContent PROGMEM = R"(
<!DOCTYPE html>
<html>
<head>
  <title>Clicker Games</title>
  <script>
    if (!!window.EventSource) {
      var source = new EventSource('/events');

      source.addEventListener('open', function(e) {
        console.log("Events Connected");
      }, false);

      source.addEventListener('error', function(e) {
        if (e.target.readyState != EventSource.OPEN) {
          console.log("Events Disconnected");
        }
      }, false);

      source.addEventListener('message', function(e) {
        console.log("message", e.data);
      }, false);

      source.addEventListener('heartbeat', function(e) {
        console.log("heartbeat", e.data);
      }, false);

      source.addEventListener('keyState', function(e) {
        console.log("keyState", e.data);
      }, false);
    }
  </script>
</head>
<body>
  <h1>Clicker Games</h1>
</body>
</html>
)";