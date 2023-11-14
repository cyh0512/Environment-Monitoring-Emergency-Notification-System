var token = "Ui0tCLVIFAXBTew1LRDrZzxZAat0G0NZXQuifUb749c";
var siteName = "臺南";  //觀測站名稱，上行政院環保署環境資料開放平台查詢，網址：https://opendata.epa.gov.tw/Data/Contents/AQI

var checkTime = [[8, 0],
                 [12, 0]];  //查詢空氣品質的時間，每組陣列第一個值為「時」、第二個值為「分」
var AQIbehavior = [[20, 0], 
                   [150, "AQI 達橘色等級，請學校減少敏感性族群學生戶外活動"],
                   [200, "AQI 達紅色等級，得將課程活動調整於室內或延期辦理"],
                   [300, "AQI 達紫色等級，立即停止戶外活動，課程調整室內進行"],
                   [500, "AQI 達深褐色等級，達停課標準"]];

function checkAQI() {
  var message;
  var DateTime = new Date();
  var nowHour = DateTime.getHours();
  var nowMinute = DateTime.getMinutes();
  var isCheckTime = 0;
  
  for (var i = 0; i < checkTime.length; i++) {
    if (nowHour == checkTime[i][0] && nowMinute == checkTime[i][1]) {
      isCheckTime = 1;
      break;
    }
  }
  if (isCheckTime == 0) { return; }
  
  var airData = JSON.parse(UrlFetchApp.fetch("http://opendata.epa.gov.tw/webapi/Data/REWIQA/?$filter=SiteName eq '" + siteName + "'&$orderby=SiteName&$skip=0&$top=1000&format=json").getContentText());
  console.log(airData);
  var AQI = airData[0].AQI;
  console.log(AQI);

  for (var i = 0; i < AQIbehavior.length; i++) {
    message = AQIbehavior[i][1];
    if (AQI <= AQIbehavior[i][0]) { break; }
  }

  if (message == 0) { return; }
  message = "\n" + siteName + "觀測站 " + message;
  for (var i = 0; i < 1; i++) {
    sendLineNotify(message, token);
    Utilities.sleep(1000);
  }    
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