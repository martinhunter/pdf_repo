# app.py

from flask import Flask
from celery import Celery
from celery.schedules import crontab

app = Flask(__name__)

# 配置Celery
app.config['CELERY_BROKER_URL'] = 'redis://<redis_server_ip>:6379/0'  # 请替换<redis_server_ip>为Redis服务器的IP地址
app.config['CELERY_RESULT_BACKEND'] = 'redis://<redis_server_ip>:6379/0'  # 请替换<redis_server_ip>为Redis服务器的IP地址

# 创建Celery实例
celery = Celery(app.name, broker=app.config['CELERY_BROKER_URL'])
celery.conf.update(app.config)

# 创建定时任务
@celery.task
def my_task():
    # 执行任务的代码
    print('Hello, Celery!')

# 创建Flask路由
@app.route('/')
def index():
    # 触发任务，这里设置每2分钟触发一次
    my_task.apply_async(countdown=120)  # 使用apply_async方法设置延迟时间

    return 'Task triggered!'

if __name__ == '__main__':
    app.run()
