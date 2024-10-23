function doPost(e) {
    var sheet = SpreadsheetApp.getActiveSpreadsheet().getSheetByName("Sheet1"); 
    var data = JSON.parse(e.postData.contents);
    
    var times = data.times;
    var message = data.message;
    var timestamp = new Date();
    
    sheet.appendRow([timestamp, times, message]);
    
    return ContentService.createTextOutput(JSON.stringify({"status": "success"})).setMimeType(ContentService.MimeType.JSON);
  }
  