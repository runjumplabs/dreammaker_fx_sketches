const hljs = require('highlight.js');
var fs = require('fs');

// hljs.registerLanguage('cpp', require('highlight.js/lib/languages/cpp'));
hljs.registerLanguage('arduino', require('highlight.js/lib/languages/arduino'));
// Open HTML template
html_template = fs.readFileSync(__dirname+"/html_template.html", 'utf8');
if (html_template.length == 0) {
	console.log("ERROR: html template did not open successfully")
}

// Ensure all of our arguments are present
if (process.argv.length < 4) {
	console.log("Missing Arduino file name");	
} else {
	var filename_in = process.argv[2];
	var filename_out = process.argv[3];

	// console.log("File in: "+filename_in)
	// console.log("File out: "+filename_out)
	var source_code = fs.readFileSync(filename_in, 'utf8');

	if (source_code.length == 0) {
    	console.log("Could not open input file")
	} else {
		
		highlightedCode = hljs.highlight('arduino', source_code).value;
		highlightedCode = html_template.replace("__CODE__",highlightedCode);
		fs.writeFileSync(filename_out, highlightedCode);
		console.log("successfully created syntax highlihted code")
	}
}
