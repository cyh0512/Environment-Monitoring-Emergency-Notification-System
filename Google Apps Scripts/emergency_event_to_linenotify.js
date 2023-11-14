function myFunction(e) {
  var token = "Ui0tCLVIFAXBTew1LRDrZzxZAat0G0NZXQuifUb749c";
  var spreadSheet = SpreadsheetApp.getActive();

  var message = "\n\n" + e.values[0] + "\n\n事件：";
      message += e.values[1] 
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