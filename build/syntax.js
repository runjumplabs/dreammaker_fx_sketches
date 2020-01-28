const hljs = require('highlight.js');
var fs = require('fs');

hljs.registerLanguage('cpp', require('highlight.js/lib/languages/cpp'));

path = __dirname+"/html_template.html"
fs.access(path, fs.constants.R_OK | fs.constants.W_OK, (err) => {
    if (err) {
        console.log("%s doesn't exist", path);
    } else {
        console.log('can read/write %s', path);
    }
});

html_template = ""
fs.readFile(__dirname+"/html_template.html", 'utf8', function (err, data) {
	if (err) {
		console.log("could not open template")
		.exit()
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
			
			highlightedCode = hljs.highlight('cpp', data).value;
			highlightedCode = html_template.replace("__CODE__",highlightedCode);

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
