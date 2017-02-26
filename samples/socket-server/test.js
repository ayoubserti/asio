var http = require('http');

function Test(callback) {

    return http.get("http://127.0.0.1:8100/", function(response) {
        // Continuously update stream with data
       response.on('data', function(d) {
            console.log(d.toString());
        });
        response.on('end', function() {})
    }).on("error",function(e)
    {console.log(e)}).on("end",function(){
        console.log("end");
    });

}
let i =100000;
while(--i)
    Test()
