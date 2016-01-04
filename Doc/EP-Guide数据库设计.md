##数据库设计

#####用户表(user)
- username (主键)
- password (MD5+SHA1双重加密,初次登录或重新登录验证)
- token (二次登录验证)
- car_ID (车牌号)

#####车库表(garage)
- garage_ID (主键)
- isFull (是否已满)

#####车位表(parking_spaces)
- ID (主键)
- park_ID （停车位ID）
- garage_ID （车库ID）
- status (车位状态,空闲or占用?)
- start_time (停车开始时刻)

#####历史停车记录表(history_park)
- ID (主键)
- park_ID （停车位ID）
- garage_ID （车库ID）
- getIn_time(入库时间)
- park_time (停车时长)