//
//  ContentView.swift
//  Vroomba
//
//  Created by Mate Tohai on 25/09/2023.
//

import SwiftUI


// copy-pasted code as im lazy
struct CircularSliderView: View {
    @Binding var progress: Double
    
    @State private var rotationAngle = Angle(degrees: 0)
    private var minValue = 0.0
    private var maxValue = 1.0
    
    init(value progress: Binding<Double>, in bounds: ClosedRange<Int> = 0...1) {
        self._progress = progress
        
        self.minValue = Double(bounds.first ?? 0)
        self.maxValue = Double(bounds.last ?? 1)
        self.rotationAngle = Angle(degrees: progressFraction * 360.0)
    }
    
    private var progressFraction: Double {
        return ((progress - minValue) / (maxValue - minValue))
    }
    
    private func changeAngle(location: CGPoint) {
        // Create a Vector for the location (reversing the y-coordinate system on iOS)
        let vector = CGVector(dx: location.x, dy: -location.y)
        
        // Calculate the angle of the vector
        let angleRadians = atan2(vector.dx, vector.dy)
        
        // Convert the angle to a range from 0 to 360 (rather than having negative angles)
        let positiveAngle = angleRadians < 0.0 ? angleRadians + (2.0 * .pi) : angleRadians
        
        // Update slider progress value based on angle
        progress = ((positiveAngle / (2.0 * .pi)) * (maxValue - minValue)) + minValue
        rotationAngle = Angle(radians: positiveAngle)
    }
    
    var body: some View {
        GeometryReader { gr in
            let radius = (min(gr.size.width, gr.size.height) / 2.0) * 0.9
            let sliderWidth = radius * 0.170
            
            VStack(spacing:0) {
                ZStack {
                    Circle()
                        .trim(from: 0, to: progressFraction)
                        .stroke(Color(hue: 0.0, saturation: 0.5, brightness: 0.9),
                                style: StrokeStyle(lineWidth: sliderWidth, lineCap: .round)
                        )
                        .rotationEffect(Angle(degrees: -90))
                    
                    Circle()
                        .fill(Color.white)
                        .shadow(radius: (sliderWidth * 0.3))
                        .frame(width: sliderWidth, height: sliderWidth)
                        .offset(y: -radius)
                        .rotationEffect(rotationAngle)
                        .gesture(
                            DragGesture(minimumDistance: 0.0)
                                .onChanged() { value in
                                    changeAngle(location: value.location)
                                }
                        )
                }
                .frame(width: radius * 2.0, height: radius * 2.0, alignment: .center)
                .padding(radius * 0.1)
            }
            
            // only original thing
            .onChange(of: progressFraction) {
                var actualProgress = progressFraction
                var left: Float = 0
                var right: Float = 0
                
                var currentSector = floor(actualProgress/0.25)
                
                if currentSector == 0 {
                    left = 1
                    right = Float(1 - actualProgress * 4)
                } else if currentSector == 1 {
                    left = Float(1 - (actualProgress-0.25) * 4)
                    right = 0
                } else if currentSector == 2 {
                    left = 0
                    right = Float((actualProgress-0.5) * 4)
                } else if currentSector == 3 {
                    left = Float((actualProgress-0.75) * 4)
                    right = 1
                }
                print("left", left, "right", right)
                
                sendMotorPositions(right: Int(right*255), left: Int(left*255))
            }
            
            .onAppear {
                self.rotationAngle = Angle(degrees: progressFraction * 360.0)
            }
        }
    }
}


struct ContentView: View {
    @State var right: Float = 0
    @State var left: Float = 0
    @State var average: Float = 0
    
    @State var value: Double = 0
    
    var body: some View {
        VStack {
            CircularSliderView(value: $value)
            
            Slider(value: $left, in: -255...255, step: 20, onEditingChanged: {_ in
                sendMotorPositions(right: Int(right), left: Int(left))
                average = (left + right) / 2
            })
            
            Spacer()
            
            Button(action: {
                right = 0
                left = 0
                average = 0
                sendMotorPositions(right: 0, left: 0)
            }, label: {
                Text("Reset")
            })
            
            Spacer()
            
            Slider(value: $average, in: -255...255, step: 20, onEditingChanged: {_ in
                sendMotorPositions(right: Int(average), left: Int(average))
                left = average
                right = average
            })
            
            Spacer()
            
            Slider(value: $right, in: -255...255, step: 20, onEditingChanged: {_ in
                sendMotorPositions(right: Int(right), left: Int(left))
                average = (left + right) / 2
            })
        }
        .padding()
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
