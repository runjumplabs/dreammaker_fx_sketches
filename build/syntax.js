const hljs = require('highlight.js');
hljs.registerLanguage('arduino', require('highlight.js/lib/languages/arduino'));
var fs = require('fs');


var html_template;
fs.readFile("html_template.html", 'utf8', function (err, data) {
	if (err) {
		console.log("could not open template")
	}
	html_template = data
});

if (process.argv.length < 4) {
	console.log("Missing Arduino file name");	
} else {
	var filename_in = process.argv[2];
	var filename_out = process.argv[3];

	console.log("File in: "+filename_in)
	console.log("File out: "+filename_out)
	fs.readFile(filename_in, 'utf8', function (err, data) {
	  	if (err) {
	    	console.log("Could not open input file")
		} else {
			data = html_template.replace("__CODE__",data);
			console.log(data);
			var highlightedCode = hljs.highlight('arduino', data).value;
			fs.writeFile(filename_out, highlightedCode, (err) => {
			    if (err) {
			    	console.log("Could not write to output file")
			    } else {			    	
				    console.log('Success!');
			    }

			});
		}
	});

}
