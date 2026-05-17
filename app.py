import os

from flask import Flask, jsonify, request, send_from_directory

from bridge import call_bridge


BASE_DIR = os.path.dirname(os.path.abspath(__file__))

app = Flask(__name__, static_folder=None, template_folder=None)


@app.get("/")
def index():
    return send_from_directory(BASE_DIR, "index.html")


@app.get("/style.css")
def style():
    return send_from_directory(BASE_DIR, "style.css")


@app.get("/script.js")
def script():
    return send_from_directory(BASE_DIR, "script.js")


@app.post("/api/<function_name>")
def api(function_name):
    payload = request.get_json(silent=True) or {}
    try:
        result = call_bridge(function_name, payload)
        status_code = 200 if result.get("status") == "ok" else 400
        return jsonify(result), status_code
    except Exception as exc:
        return jsonify({"status": "error", "message": str(exc)}), 500


if __name__ == "__main__":
    app.run(host="127.0.0.1", port=5000, threaded=False)
