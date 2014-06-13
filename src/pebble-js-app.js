// https://github.com/pyro2927/Pebble-Quick-Config

Pebble.addEventListener("showConfiguration",
  function(e) {
    Pebble.openURL("http://pebble-config.herokuapp.com/config?title=Quick%20Config&fields=email_Username,password_Passwd"); // our dyanmic configuration page
  }
);
Pebble.addEventListener("webviewclosed",
  function(e) {
    var configuration = JSON.parse(e.response);
    Pebble.sendAppMessage(configuration);
  }
);