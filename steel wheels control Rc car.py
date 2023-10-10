from inputs import get_gamepad
import serial
import socket

# Define your input variable range
min_input = -32767
max_input = 32767

# Define the degree range for the servo motor
min_degree = 45
max_degree = 135
button_lt = 0
button_rt = 0

# Arduino serial port configuration
arduino_port = 'COM12'  # Replace 'COMX' with the actual port name of your Arduino
arduino_baudrate = 9600

# NodeMCU server configuration
nodemcu_ip = '192.168.0.101'  # Replace with the actual IP address of your NodeMCU
nodemcu_port = 80

# Establish serial connection to Arduino
arduino_serial = serial.Serial(arduino_port, arduino_baudrate)

# Establish socket connection to NodeMCU
nodemcu_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
nodemcu_socket.connect((nodemcu_ip, nodemcu_port))

# Function to map the input variable to the degree range
def map_to_degree(input_value):
    # Calculate the slope and intercept for the linear mapping
    slope = (max_degree - min_degree) / (max_input - min_input)
    intercept = min_degree - slope * min_input
    
    # Use the linear equation to map the input_value to the degree range
    degree = slope * input_value + intercept
    
    # Ensure the result is within the valid degree range
    degree = max(min_degree, min(max_degree, degree))
    print(degree)
    
    return degree


#print(f"Servo degree: {servo_degree}")
def main():
    global button_lt
    global button_rt
    #Just print out some event infomation when the gamepad is used.
    while True:
        events = get_gamepad()
        for event in events:
            #print(event.ev_type, event.code, event.state)
            if event.code == "ABS_X":
                slope = (max_degree - min_degree) / (max_input - min_input)
                intercept = min_degree - slope * min_input
                degree = slope * event.state + intercept
                degree = max(min_degree, min(max_degree, degree))
                degree = degree//1
                degree = int(degree)
                print("Steering wheel: ", degree)
            elif event.code == "ABS_Z":
                button_lt = event.state
                print("LT: ", event.state)
            elif event.code == "ABS_RZ":
                button_rt = event.state
                print("RT: ", event.state)
        data = str(degree)+'_'+str(button_lt)+'_'+str(button_rt)+"\n"
        nodemcu_socket.sendall(data.encode('utf-8'))
        print(data)       

if __name__ == "__main__":
    main()

