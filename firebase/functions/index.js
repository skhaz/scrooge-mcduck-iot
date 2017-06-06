const functions = require('firebase-functions');
const admin = require('firebase-admin');
const request = require('request-promise');
const moment = require('moment-timezone');
const buildUrl = require('build-url');

admin.initializeApp(functions.config().firebase);


const API_URL = buildUrl('http://apilayer.net', {
  path: 'api/live',
  queryParams: {
    access_key: functions.config().apilayer.key,
    currencies: 'BRL',
    source: 'USD',
    format: 1
  }
});

exports.hourly_job =
  functions.pubsub.topic('hourly-tick').onPublish((event) => {
    return request({
      uri: API_URL,
      resolveWithFullResponse: true,
    }).then(response => {
      if (response.statusCode === 200) {
        const result = JSON.parse(response.body);
        return admin.database().ref('/currency')
          .set({
            currency: result.quotes.USDBRL,
            timestamp: result.timestamp
          });
      }
      throw response.body;
    });
  });

exports.format_display = functions.database.ref('/currency')
  .onWrite(event => {
    const currency = event.data.val().currency;
    const timestamp = event.data.val().timestamp;
    admin.database().ref('/settings').once('value').then(snapshot => {
      const settings = snapshot.val();
      const updated_on = moment.unix(timestamp)
        .tz('America/Sao_Paulo')
        .format('DD-MM-YYYY HH:mm:ss');

      return event.data.ref.parent.child('display').set({
        transfer: (currency * settings.transfer).toFixed(2),
        payment: (currency * settings.payment).toFixed(2),
        withdraw: (currency * settings.withdraw).toFixed(2),
        updated_on: updated_on
      });
    });
  });
