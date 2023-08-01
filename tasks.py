# tasks.py

from celery import Celery

app = Celery('tasks',broker = 'redis://:foobar@192.168.31.126:6379/1',
backend = 'redis://:foobar@192.168.31.126:6379/2')
# app.config_from_object('config')

@app.task
def send_task():
    # 在这里编写您的定时任务逻辑
    result = "任务执行成功"
    print(result)
    abc(send_task)
    return result

def abc(task):
    print('--',task, task.request.id, '------')
