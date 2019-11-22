import React from 'react';
import { View, Text, Button, Modal, TouchableHighlight} from 'react-native';
import { createAppContainer } from 'react-navigation';
import { createStackNavigator } from 'react-navigation-stack';

import Sensores from './src/pages/Sensores'

class HomeScreen extends React.Component {
    
    state = {
        modalVisible: false,
    };

    setModalVisible(visible) {
      this.setState({modalVisible: visible});
    }
    render() {
      return (
        <View style={{ flex: 1, alignItems: 'center', justifyContent: 'center', backgroundColor: '#562855'}}>
          <Text style={{fontSize: 42, fontWeight: 'bold', color: 'white'}}>iH4u</Text>
          <Button
            color='#8a3f88'
            title="Ver meus sensores"
            onPress={() => this.props.navigation.navigate('Sensores')}
          />
          <View style={{marginTop: 5}}>
            <Button
              color='#8a3f88'
              title="Sobre o iH4u"
              onPress={() => this.setModalVisible(true)}
            />
          </View>
          <Modal
              animationType="slide"
              transparent={false}
              visible={this.state.modalVisible}
              onRequestClose={() => {
                  this.setModalVisible(!this.state.modalVisible);
              }}
          >
              <View style={{flex: 1, alignItems: 'center', justifyContent: 'center', backgroundColor: '#8a3f88'}}>
                <Text style={{color: 'white', fontSize: 28, alignSelf: 'center', fontWeight: 'bold'}}>I Hear For You</Text>
                <Text style={{color: 'white', fontSize: 18, alignSelf: 'center', flexWrap: 'wrap', margin: 20}}>
                  Projeto desenvolvido na disciplina de Internet das Coisas da Universidade Federal do Ceará,
                  com o intuito de ajudar deficientes auditivos em atividades cotidianas como saber que alguém tocou
                  sua campainha ou que algum objeto caiu em algum cômodo da casa.
                </Text>
                <View style={{flexDirection: 'column', marginVertical: 5}}>
                  <Text style={{color: 'white', fontSize: 15, alignSelf: 'center', fontWeight: 'bold'}}>Desenvolvedores</Text>
                  <Text style={{color: 'white', fontSize: 15, alignSelf: 'center'}}>airtonfilho@alu.ufc.br</Text>
                  <Text style={{color: 'white', fontSize: 15, alignSelf: 'center'}}>josejoaosilva.n@gmail.com</Text>
                  <Text style={{color: 'white', fontSize: 15, alignSelf: 'center'}}>marcelfonteles@gmail.com</Text>
                  <Text style={{color: 'white', fontSize: 15, alignSelf: 'center'}}>victoriarviana@gmail.com</Text>
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
      );
    }
  }

const AppNavigator = createStackNavigator(
{
  Home: {
    screen: HomeScreen,
  },
  Sensores: {
    screen: Sensores,
  },
},
{
  headerMode: 'false',
  navigationOptions: { headerVisible: false },
},
);

export default createAppContainer(AppNavigator);