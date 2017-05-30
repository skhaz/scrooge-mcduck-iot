import webapp2
import time
import pubsub_utils


class PushToPubSub(webapp2.RequestHandler):
    def get(self, topic):
        if 'X-AppEngine-Cron' not in self.request.headers:
            self.error(403)
            return

        pubsub_utils.publish_to_topic(topic, str(time.time()))
        self.response.status_int = 204

app = webapp2.WSGIApplication([
    webapp2.Route(r'/publish/<topic>', handler=PushToPubSub)
], debug=True)
