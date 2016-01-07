##数据库设计

#####用户表(user)
- username (主键) （String）
- password (MD5+SHA1双重加密,初次登录或重新登录验证) （String）
- token (二次登录验证) （String）
- car_ID (车牌号) （String）

#####车库表(garage)
- garage_ID (主键)
- garage_name (车库名)
- isFull (是否已满)
- gtwMac （网关mac地址）

#####车位表(parking_spaces)
- ID (主键) (int)
- park_ID （停车位ID） （String）
- park_spaceName (车位名) （String）
- garage_ID （车库ID） (int)
- status (车位状态,空闲or占用?) (int)
- car_ID (所停车的ID) (String)
- start_time (停车开始时刻) (存时间戳)(long)

#####历史停车记录表(history_park)
- ID (主键) (int)
- park_ID （停车位ID） （String）
- garage_ID （车库ID） （String）
- getIn_time(入库时间) (存时间戳)(long)
- park_time (停车时长) (存时间戳)(long)