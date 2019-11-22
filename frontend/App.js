import React, {Fragment} from 'react';
import Routes from './routes';
import * as Font from 'expo-font';

export default class App extends React.Component {

  componentDidMount() {
    Font.loadAsync({
      'AbhayaLibre-Bold': require('./assets/fonts/AbhayaLibre-Bold.ttf'),
    });
  }

  render(){
    return (
        <Routes />
    );
  }
}