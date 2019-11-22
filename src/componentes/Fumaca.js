import React, { Component } from 'react';
import {
    Text,
    View,
    Vibration,
    AsyncStorage,
    Modal,
    TouchableHighlight
  } from 'react-native';
import Icon from 'react-native-vector-icons/MaterialCommunityIcons';
import Switch from '../componentes/Switch'

const DURATION = 4000;
const PATTERN = [500, 1000];

import init from 'react_native_mqtt';

init({
  size: 10000,
  storageBackend: AsyncStorage,
  defaultExpires: 1000 * 3600 * 24,
  enableCache: true,
  sync: {},
});

const client = new Paho.MQTT.Client('broker.mqttdashboard.com', 8000, 'clientId-H2doUPxBtC',);

export default class Fumaca extends Component {
    
    state = {
        modalVisible: false,
        on: false,
    };

    componentDidMount(){
        this.initialize()
    }

    componentWillUnmount(){
        client.disconnect();
    }

    setModalVisible(visible) {
      this.setState({modalVisible: visible});
    }

    onMessageArrived = (entry) => {
        console.log("onMessageArrived:"+entry);
        console.log(entry.payloadString);
        Vibration.vibrate(PATTERN, true);
        this.props.notification('Hey, tem fumaça na cozinha!!')
        this.setModalVisible(!this.state.modalVisible);
    }

    onConnectionLost = (err) => {
        this.setState({on: false})
        console.log(err)
    }

    onConnect = () => {
        this.setState({on: true})
        client.subscribe('ih4u/sensores/pub/smoke');
    };

    sendMessage = (message) => {
        message = new Paho.MQTT.Message(message);
        message.destinationName = "ih4u/sensores/sub/smoke";
        client.send(message);
    }

    initialize = () => {
        client.onMessageArrived = this.onMessageArrived;
        client.onConnectionLost = this.onConnectionLost;
        client.connect({ 
        onSuccess: this.onConnect,
        useSSL: false ,
        userName: 'yourUser',
        password: 'yourPass',
        onFailure: (e) => {console.log("here is the error" , e); }
        });
    }

    render(){    
        return( 
            <View style={{ flex: 1, alignItems: 'center', justifyContent: 'center', backgroundColor: '#562855'}}>
                <View style={{flexDireaction: 'column', borderColor: 'white', borderWidth: 2, borderRadius: 5, paddingHorizontal: 10, paddingVertical: 5, minWidth: 250}}>
                    <View style={{justifyContent: 'flex-end', flexDirection: 'row'}}>
                    <Icon name="replay" size={20} color="white" />
                    </View>
                    <View style={{flexDirection: 'row', alignContent: 'space-around'}}>
                        <Icon style={{alignSelf: 'center', marginRight: 10}} name="fire" size={60} color="white" />
                        <View style={{flexDirection: 'column', justifyContent: 'center', alignItems: 'center'}}>
                            <Text style={{color: 'white', margin: 20, fontSize: 20, fontWeight: 'bold'}}>Fumaça</Text>
                            <Switch value={this.state.on}/>
                        </View>
                    </View>
                </View>
                <Modal
                    animationType="slide"
                    transparent={false}
                    visible={this.state.modalVisible}
                    onRequestClose={() => {
                        this.setModalVisible(!this.state.modalVisible);
                    }}
                >
                    <View style={{flex: 1, alignItems: 'center', justifyContent: 'center', backgroundColor: '#FF291D'}}>
                            <Text style={{color: 'white', fontSize: 24, alignSelf: 'center'}}>Hey, tem fumaça na cozinha!!</Text>

                            <TouchableHighlight
                                style={{backgroundColor: 'white',borderRadius: 3, padding: 5, elevation: 5}}
                                onPress={() => {
                                    Vibration.cancel();
                                    this.setModalVisible(!this.state.modalVisible);
                                }}
                            >
                                <Text style={{color: '#fc6e65', fontSize: 22}}>Opa! Vou verificar!</Text>
                            </TouchableHighlight>
                    </View>
                </Modal>
            </View>
        )
    }
}
