import cv2
import pyttsx3
import serial
import time
import threading

KNOWN_WIDTH = 50  
FOCAL_LENGTH = 615  
# Initialize Text-to-Speech
engine = pyttsx3.init()
engine.setProperty('rate', 200)  
engine.setProperty('volume', 4.0)  
def speak(message):
    def _speak():
        engine.say(message)
        engine.runAndWait()
    threading.Thread(target=_speak, daemon=True).start()


try:
    arduino = serial.Serial('COM3', 9600, timeout=1)  
    time.sleep(2)
except:
    print("Could not connect to Arduino.")
    arduino = None


cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_upperbody.xml')

cap = cv2.VideoCapture(0)
print("Detecting... Press 'q' to quit.")
last_alert_time = 0
alert_delay = 5  

while True:
    ret, frame = cap.read()
    if not ret:
        break

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    bodies = cascade.detectMultiScale(
    gray, 
    scaleFactor=1.1, 
    minNeighbors=4,
    minSize=(60, 60),   
    maxSize=(300, 300) 
)
    current_time = time.time()

    if len(bodies) > 0 and current_time - last_alert_time > alert_delay:
        print("Person detected")

        engine.say("Person detected ahead")
        engine.runAndWait()
        if arduino:
            arduino.write(b'P')

        last_alert_time = current_time

    for (x, y, w, h) in bodies:
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
        cv2.putText(frame, "Person", (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)

    cv2.imshow("Smart Blind Stick - Person Detection", frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
if arduino:
    arduino.close()
