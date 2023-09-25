//
//  ContentView.swift
//  Vroomba
//
//  Created by Mate Tohai on 25/09/2023.
//

import SwiftUI

struct ContentView: View {
    @State var right: Float = 0
    @State var left: Float = 0
    @State var average: Float = 0
    
    var body: some View {
        VStack {
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
