from flask import Flask, request, render_template_string
import os

app = Flask(__name__)

# Set the folder to store uploaded files
UPLOAD_FOLDER = 'uploads'
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER

# Allowed file extensions
ALLOWED_EXTENSIONS = {'txt', 'pdf', 'png', 'jpg', 'jpeg', 'gif'}

# Function to check allowed file extensions
def allowed_file(filename):
    return '.' in filename and filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS

# HTML template for the file upload form
html_form = '''
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Upload Your File</title>
</head>
<body>
    <div class="container">
        <h1>Upload Your File</h1>
        <form action="/" method="POST" enctype="multipart/form-data">
            <input type="file" name="file" id="myFile">
            <input type="submit" value="Upload">
        </form>
        {% if filename %}
        <h2>File successfully uploaded: {{ filename }}</h2>
        {% endif %}
    </div>
</body>
</html>
'''

@app.route('/', methods=['GET', 'POST'])
def upload_file():
    if request.method == 'POST':
        if 'file' not in request.files:
            return 'No file part'
        file = request.files['file']
        
        if file.filename == '':
            return 'No selected file'
        
        if file and allowed_file(file.filename):
            filename = os.path.join(app.config['UPLOAD_FOLDER'], file.filename)
            file.save(filename)
            return render_template_string(html_form, filename=file.filename)
    
    return render_template_string(html_form, filename=None)

if __name__ == '__main__':
    # Make sure the upload folder exists
    if not os.path.exists(UPLOAD_FOLDER):
        os.makedirs(UPLOAD_FOLDER)
    
    app.run(debug=True)
