var http = require('http');
var url = require('url');
var fs = require('fs');
var qs = require('querystring')
var MongoClient = require('mongodb').MongoClient;
var url = "mongodb://localhost:27017/";
var auth_key="Bb123456789"
var filename = "./files/rfid.htm";
var datacollectordb = "datacollectordb"
function formatDate(date) {
  var hours = date.getHours();
  var minutes = date.getMinutes();
  var seconds = date.getSeconds();
  var ampm = hours >= 12 ? 'pm' : 'am';
  hours = hours % 12;
  hours = hours ? hours : 12; // the hour '0' should be '12'
  minutes = minutes < 10 ? '0'+minutes : minutes;
  seconds = seconds < 10 ? '0'+seconds : seconds;
  var strTime = hours + ':' + minutes + ':' + seconds + " "+ampm;
  return date.getDate() + "/" + (date.getMonth()+1) + "/" + date.getFullYear() + "  " + strTime;
}

http.createServer(function (request, response) {
	var j=0;
	//var q = url.parse(request.url, true).query;
    if (request.method == 'POST') {
		console.log("Post request occured");
        // save all data received
        var postdata = '';

        // receiving data
        request.on('data', function(chunk) {
			x= new Date()
            postdata += chunk;
            // Avoid too much POST data
            if (postdata.length > 1e6)
                request.connection.destroy();
        });

        // received all data
        request.on('end', function() {
			//console.log((new Date())-x);
			console.log(postdata);
			//textArray = JArray.Parse(postdata);
            //var post = qs.parse(postdata);
			var post = JSON.parse(String(postdata));
			//console.log("Post requist ended");
            // handle post by accessing
			MongoClient.connect(url, function(err, db) {
				if (err) throw err;
				var dbo = db.db(datacollectordb);
				var date = new Date();

				for(i=0;i<post.length;i++){
				var sensObject = post[i];
				var myobj = { time: formatDate(date), value: String(sensObject['Value']) };
				dbo.collection(String(sensObject['SensorID'])).insertOne(myobj, function(err, res) {
				if (err) throw err;
				//console.log("document inserted successfully");
				});
				}
				db.close();
			});
			//console.log((new Date())-x);
			//console.log( post.length);
			response.writeHead(200, {'Content-Type': 'application/json'});
            response.write(JSON.stringify({ response: " done" }));
			response.end();
        });

    } else if (request.method == 'GET') {
		MongoClient.connect(url, function(err, db) {
			if (err) throw err;

			var dbo = db.db(datacollectordb);
			var db_names ;
			dbo.listCollections().toArray(function(err, collections){
				//console.log(collections.length );
				db_names =collections;
				response.setHeader("Access-Control-Allow-Origin", "*",
        "Connection","*",
      "Keep-Alive","timeout=2, max=7000 ");
				response.writeHead(200, {'Content-Type': 'application/json'});
				response.write("[");

				for( var i=0;i<collections.length;i++){
					var mysort = { _id: -1 };
					var temp_name= db_names[i];
					var realname = temp_name['name'];
					//console.log(realname );

					dbo.collection(String(realname)).find({}).sort(mysort).limit(1).toArray(function(err, result) {
						if (err) throw err;
						j++;
						//console.log(result[0]);
						//console.log(collections.length-1);
						//console.log(j);

						if(j==(collections.length)) {
							response.write(JSON.stringify(result[0])+']');
							response.end();
						}
						else {
							response.write(JSON.stringify(result[0])+',');
						}
					});
				}
				db.close();

			});

		});
        console.log("get request occured");
    }
    return
}).listen(8680);
