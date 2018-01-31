// https://github.com/pyro2927/Pebble-Quick-Config

Pebble.addEventListener("showConfiguration",
  function(e) {
    Pebble.openURL("https://carolmenezes3.github.io/mypebblenotessettings.html"); // our dyanmic configuration page
  }
);
Pebble.addEventListener("webviewclosed",
  function(e) {
    var configuration = JSON.parse(e.response);
    Pebble.sendAppMessage(configuration);
  }
);
