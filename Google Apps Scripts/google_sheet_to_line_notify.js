function myFunction(e) {
  var token = "Ui0tCLVIFAXBTew1LRDrZzxZAat0G0NZXQuifUb749c";
  var spreadSheet = SpreadsheetApp.getActive();

  var message = "\n\n" + e.values[0] + "\n\n事件：";
      message += e.values[1] + "\n\n溫度：";
      message += e.values[2] + "\n\n濕度：";
      message += e.values[3] + "\n\n電話通報：";
      message += e.values[4] + "\n\n空氣品質指數AQI：";
      message += e.values[5] + "\n\n紫外線指數UVI：";
      message += e.values[6];
  sendLineNotify(message, token);
}

function sendLineNotify(message, token){
  var options =
   {
     "method"  : "post",
     "payload" : {"message" : message},
     "headers" : {"Authorization" : "Bearer " + token}
   };
   UrlFetchApp.fetch("https://notify-api.line.me/api/notify", options);
}