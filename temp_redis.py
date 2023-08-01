import redis

# 创建Redis客户端连接
redis_host = '192.168.31.126'
redis_port = 6379
redis_db = 1

r = redis.Redis(host=redis_host, port=redis_port, db=redis_db, password='foobar')

# 尝试对远程Redis进行一些操作
try:
    # 设置键值对
    print('start setting key')
    r.set('test_key', 'test_value')

    # 获取键的值
    value = r.get('test_key')
    print("获取到的值:", value)

    # 删除键
    r.delete('test_key')

    print("成功连接到远程Redis并进行了一些操作。")
except redis.RedisError as e:
    print("无法连接到远程Redis:", e)
