import SwiftUI
import CoreBluetooth
import Speech
import Foundation
import AVFoundation

import SwiftUI

struct ContentView: View {
    @StateObject private var bluetoothManager = BluetoothManager()
    @StateObject private var speechManager = SpeechManager()
    @State private var isScanning = false
    @State private var isListening = false
    
    var body: some View {
        NavigationView {
            VStack(spacing: 20) {
                // Bluetooth Status
                Text(bluetoothManager.connectionStatus)
                    .font(.headline)
                    .padding()
                    .background(bluetoothManager.connectionStatus == "Connected" ? Color.green : Color.red)
                    .cornerRadius(10)
                    .foregroundColor(.white)

                Text(bluetoothManager.debug)
                    .font(.subheadline)
                    .padding()

                // Bluetooth Scanning Button
                Button(action: {
                    isScanning.toggle()
                    if isScanning {
                        bluetoothManager.startScanning()
                    } else {
                        bluetoothManager.stopScanning()
                    }
                }) {
                    Text(isScanning ? "Stop Scanning" : "Find Devices")
                        .font(.headline)
                        .padding()
                        .frame(maxWidth: .infinity)
                        .background(isScanning ? Color.red : Color.blue)
                        .cornerRadius(10)
                        .foregroundColor(.white)
                        .shadow(radius: 5)
                }

                // List of Discovered Peripherals as Buttons
                List(bluetoothManager.discoveredPeripherals, id: \.identifier) { peripheral in
                    Button(action: {
                        bluetoothManager.connectToDevice(peripheral)
                    }) {
                        HStack {
                            Text(peripheral.name ?? "Unknown")
                                .font(.body)
                                .padding()
                                .background(Color.gray.opacity(0.1))
                                .cornerRadius(10)
                        }
                    }
                }
                .listStyle(PlainListStyle())

                // Toggle Listening Button
                Button(action: {
                    if isListening {
                        speechManager.stopListening()
                    } else {
                        speechManager.startListening()
                    }
                    isListening.toggle()
                }) {
                    HStack {
                        Image(systemName: isListening ? "mic.slash.fill" : "mic.fill")
                            .font(.title)
                        Text(isListening ? "Stop Listening" : "Start Listening")
                            .font(.headline)
                    }
                    .padding()
                    .frame(maxWidth: .infinity)
                    .background(Color.orange)
                    .cornerRadius(10)
                    .foregroundColor(.white)
                    .shadow(radius: 5)
                }

                // Display Recognized Text
                Text(speechManager.recognizedText)
                    .font(.body)
                    .padding()
                    .frame(maxWidth: .infinity, alignment: .leading)
                    .background(Color.gray.opacity(0.2))
                    .cornerRadius(10)

                // Display Validity Status
                Text(speechManager.isInputValid ? "Valid Input" : "Invalid Input")
                    .font(.headline)
                    .padding()
                    .foregroundColor(speechManager.isInputValid ? .green : .red)
                    .frame(maxWidth: .infinity, alignment: .leading)

                // Clear Button
                Button(action: {
                    speechManager.recognizedText = ""
                    speechManager.isInputValid = false
                }) {
                    Text("Clear Text")
                        .font(.headline)
                        .padding()
                        .frame(maxWidth: .infinity)
                        .background(Color.gray)
                        .cornerRadius(10)
                        .foregroundColor(.white)
                        .shadow(radius: 5)
                }

                // Send Button
                Button(action: {
                    if speechManager.isInputValid {
                        bluetoothManager.sendData(speechManager.recognizedText)
                    } else {
                        print("Invalid input, cannot send data")
                    }
                }) {
                    Text("Send to Bluetooth")
                        .font(.headline)
                        .padding()
                        .frame(maxWidth: .infinity)
                        .background(speechManager.isInputValid ? Color.blue : Color.gray)
                        .cornerRadius(10)
                        .foregroundColor(.white)
                        .shadow(radius: 5)
                }

            }
            .padding()
            .navigationTitle("Voice Controlled Checkers")
            .onAppear {
                speechManager.requestSpeechRecognitionPermission()
            }
        }
    }
}



#Preview {
    ContentView()
}


class BluetoothManager: NSObject, ObservableObject, CBCentralManagerDelegate, CBPeripheralDelegate {
    @Published var connectionStatus = "Not connected"
    @Published var debug = "waiting"
    @Published var discoveredPeripherals: [CBPeripheral] = []
    private var centralManager: CBCentralManager!
    private var selectedPeripheral: CBPeripheral?
    private var writeCharacteristic: CBCharacteristic?
    
    private let serviceUUID = CBUUID(string: "6e400001-b5a3-f393-e0a9-e50e24dcca9e")
    private let writeCharacteristicUUID = CBUUID(string: "6e400002-b5a3-f393-e0a9-e50e24dcca9e")
    
    override init() {
        super.init()
        centralManager = CBCentralManager(delegate: self, queue: nil)
    }
    
    func startScanning() {
        debug = "Starting scan..."
        centralManager.scanForPeripherals(withServices: nil, options: nil)
    }
    
    func stopScanning() {
        debug = "Stopping scan..."
        centralManager.stopScan()
    }
    
    func centralManagerDidUpdateState(_ central: CBCentralManager) {
        switch central.state {
        case .poweredOn:
            debug = "Bluetooth is powered on and available."
        case .poweredOff:
            debug = "Bluetooth is powered off."
        case .resetting:
            debug = "Bluetooth is resetting."
        case .unauthorized:
            debug = "Bluetooth access is unauthorized."
        case .unsupported:
            debug = "Bluetooth is not supported on this device."
        case .unknown:
            debug = "Bluetooth state is unknown."
        @unknown default:
            debug = "A new state was introduced that is not handled."
        }
    }
    
    func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral, advertisementData: [String: Any], rssi RSSI: NSNumber) {
        // Check if the advertisement data contains the service UUID
        if let advertisedServices = advertisementData[CBAdvertisementDataServiceUUIDsKey] as? [CBUUID] {
            // Only add peripherals that advertise the desired service UUID
            if advertisedServices.contains(serviceUUID) {
                if !discoveredPeripherals.contains(where: { $0.identifier == peripheral.identifier }) {
                    discoveredPeripherals.append(peripheral)
                    debug = "Discovered peripheral with service UUID: \(peripheral.name ?? "Unknown")"
                }
            }
        }
    }

    
    func connectToDevice(_ peripheral: CBPeripheral) {
        selectedPeripheral = peripheral
        connectionStatus = "Connecting"
        peripheral.delegate = self // Ensure the delegate is set
        centralManager.connect(peripheral, options: nil)
    }

    
    func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
        connectionStatus = "Connected"
        discoveredPeripherals = [peripheral]
        peripheral.discoverServices(nil)
    }

    func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {
        if let services = peripheral.services {
            for service in services {
                peripheral.discoverCharacteristics(nil, for: service)
            }
        }
    }

    func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService, error: Error?) {
        if let characteristics = service.characteristics {
            for characteristic in characteristics {
                // Print the UUID of each characteristic found
                print("Found characteristic with UUID: \(characteristic.uuid)")

                // Optionally, you can still store the writable characteristic if needed
                if characteristic.properties.contains(.write) {
                    writeCharacteristic = characteristic
                    print("Found writable characteristic: \(characteristic.uuid)")
                }
            }
        } else {
            print("No characteristics found for service UUID: \(service.uuid)")
        }
    }


    func sendData(_ data: String) {
           guard let peripheral = selectedPeripheral else {
               print("Peripheral not connected")
               return
           }
           
           // Search for the service
           guard let service = peripheral.services?.first(where: { $0.uuid == serviceUUID }) else {
               print("Service not found for UUID: \(serviceUUID)")
               return
           }
        
        print("Service found for UUID: \(service.uuid)")
        
        if let services = peripheral.services {
            for service in services {
                print("Service found with UUID: \(service.uuid)")
                peripheral.discoverCharacteristics(nil, for: service) // Discover characteristics for each service
            }}
        
        
     

        // Search for the characteristic within the found service
        guard let characteristic = service.characteristics?.first(where: { $0.uuid == writeCharacteristicUUID }) else {
            print("Characteristic not found for UUID: \(writeCharacteristicUUID)")
            return
        }


           let dataToSend = data.data(using: .utf8)!
           peripheral.writeValue(dataToSend, for: characteristic, type: .withResponse)

           // Check if the data is valid
           let isValidCommand = isValidInput(data)
           debug = isValidCommand ? "Sent valid command: \(data)" : "Sent invalid command: \(data)"
           print(debug)
       }
    
   private func isValidInput(_ text: String) -> Bool {
    // Checkerboard valid positions (A-H for letters and 1-8 for numbers)
       let validLetters: Set<String> = ["A", "B", "C", "D", "E", "F", "G", "H"]
       let validNumbers = Set((1...8).map { String($0) })

    // Helper function to check if a given text follows the <Letter><Number> format.
    func isValidCheckerboardPosition(_ position: String) -> Bool {
        guard position.count == 2 else { return false }
        let letter = String(position.prefix(1))
        let number = String(position.suffix(1))
        return validLetters.contains(letter) && validNumbers.contains(number)
    }

    // Helper function to split the text into <Letter><Number> pairs
    func getPairs(_ text: String) -> [String] {
        var pairs: [String] = []
        var i = 0
        while i < text.count - 1 {
            let letter = text[text.index(text.startIndex, offsetBy: i)]
            let number = text[text.index(text.startIndex, offsetBy: i + 1)]
            pairs.append(String(letter) + String(number))
            i += 3  // Move to the next <Letter><Number> pair after a space
        }
        return pairs
    }

    // Helper function to generate all possible valid spacing combinations
    func generateCombinations(_ text: String) -> [String] {
        var combinations: [String] = []
        let length = text.count

        // Generate combinations with spaces at every other position
        func generate(_ current: String, _ index: Int) {
            if index >= length {
                combinations.append(current)
                return
            }

            // Try adding current character without space
            let next = String(text[text.index(text.startIndex, offsetBy: index)])
            generate(current + next, index + 1)

            // Try adding space after every <Letter><Number> combination
            if index % 2 != 0 {
                generate(current + " " + next, index + 1)
            }
        }

        generate("", 0)
        return combinations
    }

    // Generate all combinations and check if any is valid
    let combinations = generateCombinations(text)
    for combination in combinations {
        let pairs = combination.split(separator: " ")
        if pairs.allSatisfy({ isValidCheckerboardPosition(String($0)) }) {
            return true
        }
    }

    return false
}

}

class SpeechManager: NSObject, ObservableObject {
    private var speechRecognizer: SFSpeechRecognizer?
    private var recognitionRequest: SFSpeechAudioBufferRecognitionRequest?
    private var recognitionTask: SFSpeechRecognitionTask?
    private let audioEngine = AVAudioEngine()
    
    @Published var recognizedText: String = ""
    @Published var permissionStatus: SFSpeechRecognizerAuthorizationStatus = .notDetermined
    @Published var isInputValid: Bool = false  // New property to track input validity

    override init() {
        super.init()
        speechRecognizer = SFSpeechRecognizer()
        requestSpeechRecognitionPermission()
    }

    func requestSpeechRecognitionPermission() {
        SFSpeechRecognizer.requestAuthorization { authStatus in
            DispatchQueue.main.async {
                self.permissionStatus = authStatus
                if authStatus == .authorized {
                    print("Speech recognition authorized")
                } else {
                    print("Speech recognition not authorized")
                }
            }
        }
    }
    
    func startListening() {
        guard permissionStatus == .authorized else {
            print("Speech recognition permission not granted")
            return
        }

        recognitionTask?.cancel()
        recognitionTask = nil
        
        recognitionRequest = SFSpeechAudioBufferRecognitionRequest()
        guard let recognitionRequest = recognitionRequest else {
            print("Unable to create a recognition request")
            return
        }

        recognitionRequest.shouldReportPartialResults = true
        
        let inputNode = audioEngine.inputNode
        let recordingFormat = inputNode.outputFormat(forBus: 0)
        inputNode.installTap(onBus: 0, bufferSize: 1024, format: recordingFormat) { [weak self] (buffer, when) in
            self?.recognitionRequest?.append(buffer)
        }


        audioEngine.prepare()
        do {
            try audioEngine.start()
        } catch {
            print("Audio Engine could not start: \(error.localizedDescription)")
            return
        }

        recognitionTask = speechRecognizer?.recognitionTask(with: recognitionRequest) { result, error in
            if let result = result {
                let text = result.bestTranscription.formattedString
                // Check if the recognized text is a letter followed by a number
                self.isInputValid = self.isValidInput(text)  // Update validity status
                if self.isInputValid {
                    self.recognizedText = text
                    print("Recognized text: \(self.recognizedText)")
                }

                // Handle end of recognition
                if result.isFinal {
                    self.stopListening()
                }
            }

            if let error = error {
                print("Recognition error: \(error.localizedDescription)")
                self.stopListening()
            }
        }
    }

    func stopListening() {
        // Stop the audio engine only if it's running
        if audioEngine.isRunning {
            audioEngine.stop()
            recognitionRequest?.endAudio()
            audioEngine.inputNode.removeTap(onBus: 0)
        }
        
        // Cancel the recognition task and clear the request
        recognitionTask?.cancel()
        recognitionTask = nil
        recognitionRequest = nil  // Clear the recognition request
    }


   private func isValidInput(_ text: String) -> Bool {
    // Checkerboard valid positions (A-H for letters and 1-8 for numbers)
       let validLetters: Set<String> = ["A", "B", "C", "D", "E", "F", "G", "H"]
       let validNumbers = Set((1...8).map { String($0) })

    // Helper function to check if a given text follows the <Letter><Number> format.
    func isValidCheckerboardPosition(_ position: String) -> Bool {
        guard position.count == 2 else { return false }
        let letter = String(position.prefix(1))
        let number = String(position.suffix(1))
        return validLetters.contains(letter) && validNumbers.contains(number)
    }

    // Helper function to split the text into <Letter><Number> pairs
    func getPairs(_ text: String) -> [String] {
        var pairs: [String] = []
        var i = 0
        while i < text.count - 1 {
            let letter = text[text.index(text.startIndex, offsetBy: i)]
            let number = text[text.index(text.startIndex, offsetBy: i + 1)]
            pairs.append(String(letter) + String(number))
            i += 3  // Move to the next <Letter><Number> pair after a space
        }
        return pairs
    }

    // Helper function to generate all possible valid spacing combinations
    func generateCombinations(_ text: String) -> [String] {
        var combinations: [String] = []
        let length = text.count

        // Generate combinations with spaces at every other position
        func generate(_ current: String, _ index: Int) {
            if index >= length {
                combinations.append(current)
                return
            }

            // Try adding current character without space
            let next = String(text[text.index(text.startIndex, offsetBy: index)])
            generate(current + next, index + 1)

            // Try adding space after every <Letter><Number> combination
            if index % 2 != 0 {
                generate(current + " " + next, index + 1)
            }
        }

        generate("", 0)
        return combinations
    }

    // Generate all combinations and check if any is valid
    let combinations = generateCombinations(text)
    for combination in combinations {
        let pairs = combination.split(separator: " ")
        if pairs.allSatisfy({ isValidCheckerboardPosition(String($0)) }) {
            return true
        }
    }

    return false
}

}
