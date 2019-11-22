import React, { Component } from 'react';
import {
  Text,
  View,
  Vibration,
  Button,
  Modal,
} from 'react-native';
import Icon from 'react-native-vector-icons/AntDesign';



export default class Switch extends Component {
    
    render(){
        return( 
        <View style={{ flexDirection: 'row', alignItems: 'center', justifyContent: 'space-around', backgroundColor: 'white', borderRadius:  5, width: 50, height: 30}}>
           <Text>{this.props.value ? "On" : "Off"}</Text> 
           <Icon name={this.props.value? "checkcircle" : "closecircle"} size={20} color={this.props.value? "green" : "red"} />
        </View>
        )
    }
}
