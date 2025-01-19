#!/bin/bash

# Read the input data (uploaded file) from stdin
BOUNDARY=$(grep -oE 'boundary=[^;]*' <<<"$CONTENT_TYPE" | cut -d= -f2)
INPUT=$(cat) # Read the entire stdin into the INPUT variable

# Extract the file content
FILE_CONTENT=$(echo "$INPUT" | sed -n "/^--$BOUNDARY/,/^--$BOUNDARY--/p" | tail -n +4 | head -n -2)

# Output only the HTML content
cat << EOF
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Uploaded File</title>
</head>
<body>
    <h1>Uploaded File Content</h1>
    <pre>
$FILE_CONTENT
    </pre>
</body>
</html>
EOF


