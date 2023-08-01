# run.py

from tasks import app
from celery.schedules import crontab

app.conf.beat_schedule = {
    'send-task-every-minute': {
        'task': 'tasks.send_task',
        'schedule': crontab(minute='*/1'),  # 每2分钟触发一次
    },
}

# if __name__ == '__main__':
#     app.Beat().run()

