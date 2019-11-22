import React from 'react';
import {
  Text,
  View,
  Modal,
  TouchableHighlight
} from 'react-native';
import { Notifications } from 'expo';
import * as Permissions from 'expo-permissions';
import Icon from 'react-native-vector-icons/MaterialCommunityIcons';

import Campainha from '../componentes/Campainha'
import Ruido from '../componentes/Ruido'
import Fumaca from '../componentes/Fumaca'


export default class Sensores extends React.Component {
    
    state = {
        modalVisible: false,
        on: false,
    };

    setModalVisible(visible) {
      this.setState({modalVisible: visible});
    }

    componentDidMount(){
      this.askPermissions()
    }

    askPermissions = async () => {
      const { status: existingStatus } = await Permissions.getAsync(Permissions.NOTIFICATIONS);
      let finalStatus = existingStatus;
      if (existingStatus !== granted) {
        const { status } = await Permissions.askAsync(Permissions.NOTIFICATIONS);
        finalStatus = status;
      }
      if (finalStatus !== granted) {
        return false;
      }
      return true;
    };

    sendNotificationImmediately = async (body) => {
      let notificationId = await Notifications.presentLocalNotificationAsync({
        title: 'iH4u',
        body: body,
      });
    };

    render(){
      return( 
        <View style={{ flex: 1, alignItems: 'center', justifyContent: 'center', backgroundColor: '#562855'}}>
          <Text style={{marginTop: 30, fontSize: 40, color: 'white'}}>Meus sensores</Text>
          <Campainha notification={this.sendNotificationImmediately}/>
          <Ruido notification={this.sendNotificationImmediately}/>
          <Fumaca notification={this.sendNotificationImmediately}/>
          
          <Icon name="access-point" size={20} color="white" onPress={()=>this.setModalVisible(!this.state.modalVisible)}/>
          
          <Modal
              animationType="slide"
              transparent={false}
              visible={this.state.modalVisible}
              onRequestClose={() => {
                  this.setModalVisible(!this.state.modalVisible);
              }}
          >
              <View style={{flex: 1, alignItems: 'center', justifyContent: 'center', backgroundColor: '#8a3f88'}}>
                <View style={{flexDirection: 'column', marginVertical: 5}}>
                  <Text style={{color: 'white', fontSize: 15, alignSelf: 'center', fontWeight: 'bold'}}>Topicos para testes</Text>
                  <Text style={{color: 'white', fontSize: 15, alignSelf: 'center'}}>ih4u/sensores/pub/smoke</Text>
                  <Text style={{color: 'white', fontSize: 15, alignSelf: 'center'}}>ih4u/sensores/pub/sound</Text>
                  <Text style={{color: 'white', fontSize: 15, alignSelf: 'center'}}>ih4f/sensores/sub/bell</Text>
                </View>
                <TouchableHighlight
                    style={{backgroundColor: '#562855', borderRadius: 3, padding: 5}}
                    onPress={() => {
                        this.setModalVisible(!this.state.modalVisible);
                    }}
                >
                    <Text style={{color: 'white', fontSize: 20}}>Voltar</Text>
                </TouchableHighlight>
              </View>
          </Modal>
        </View>
      )
    }
}
