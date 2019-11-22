import React, { Component } from 'react';
import {
  Text,
  View,
  Vibration,
  Modal,
  TouchableHighlight,
  AsyncStorage,
} from 'react-native';
import Icon from 'react-native-vector-icons/MaterialCommunityIcons';
import Switch from '../componentes/Switch'

const DURATION = [500, 500, 500, 500, 500, 500, 500, 500];;

import init from 'react_native_mqtt';

init({
  size: 10000,
  storageBackend: AsyncStorage,
  defaultExpires: 1000 * 3600 * 24,
  enableCache: true,
  sync: {},
});

const client = new Paho.MQTT.Client('broker.mqttdashboard.com', 8000, 'clientId-H2foUPxBtC',);

export default class Campainha extends Component {
    
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
        Vibration.vibrate(DURATION);
        this.props.notification('Hey, tem alguém na porta')
        this.setModalVisible(!this.state.modalVisible);
    }

    onConnectionLost = (err) => {
        this.setState({on: false})
        console.log(err)
    }

    onConnect = () => {
        this.setState({on: true})
        client.subscribe('ih4f/sensores/pub/bell');
    };

    sendMessage = (message) => {
        message = new Paho.MQTT.Message(message);
        message.destinationName = "ih4f/sensores/sub/bell";
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
                    <Icon name="replay" size={20} color="white" onPress={()=>this.initialize()}/>
                    </View>
                    <View style={{flexDirection: 'row', alignContent: 'space-around'}}>
                        <Icon style={{alignSelf: 'center', marginRight: 10}} name="bell-ring-outline" size={60} color="white" />
                        <View style={{flexDirection: 'column', justifyContent: 'center', alignItems: 'center'}}>
                            <Text style={{color: 'white', margin: 20, fontSize: 20, fontWeight: 'bold'}}>Campainha</Text>
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
                            <Text style={{color: 'white', fontSize: 28, alignSelf: 'center'}}>Hey, tem alguém na porta</Text>

                            <TouchableHighlight
                                style={{backgroundColor: 'white',borderRadius: 3, padding: 5, elevation: 5}}
                                onPress={() => {
                                    Vibration.cancel();
                                    this.setModalVisible(!this.state.modalVisible);
                                    this.sendMessage('Ok, ja fui!')
                                }}
                            >
                                <Text style={{color: '#fc6e65', fontSize: 22}}>Já Vooou!!!</Text>
                            </TouchableHighlight>
                    </View>
                </Modal>
            </View>
        )
    }
}
