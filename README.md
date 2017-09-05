# ZhihuSpider
	
ZhihuSpider是[鸭哥私房菜](http://www.smimi.net/)网站后台的爬虫，是基于知乎网的爬虫。

### 主要提供以下功能：
	{"首页"},
	{"今日最热"},
	{"本月最热"},
	{"话题"},
	{"关注"},
	{"提问"},
	{"回答"},
	{"文章"},
	{"收藏"},
	{"关注的少量专栏"},
	{"关注的专栏"},
	{"关注的话题"},
	{"测试关注者"},
	{"测试所有关注者"},
	{"发表文章"},
	{"今日最热三十条"},
	{"发表今日最热三十条文章"},
	{"从种子用户开始获取所有用户"},
	{"从准备队列中开始获取所有用户"},
	{"专用测试"},
	{"子话题"},
	{"关注错误重试"},
	{"更新用户信息"},
	{"拉取子话题的精华回答"},
	{"更新子话题的精华回答"},
	{"精华话题json输出"},
	{"风云人物json输出"},
	{"统计数据json输出"},
	{"登录知乎并获取cookies"},
	{"随机发表精华话题到鸭哥推荐"},
	{"更新单个用户信息"},

### 用户信息爬取思路: ###
	取一个优质种子（粉丝多，关注多）的用户来开始获取其粉丝和关注的核心信息，
	存放于redis的准备爬取队列中，然后每次在准备队列中去取特定数量的出来，
	先判断是否爬取过，若未爬取就进行爬取个人相关信息，也爬取其粉丝和关注并放入准备爬取队列中去，
	爬取完后将特定核心信息存放于已爬取的哈希redis中。
	这样的做法，会一层一层的爬取下去，基本上能爬取到所有有质量的，有参与度的用户信息。

### 话题信息爬取思路: ###
	获取知乎所有母话题id等信息，再获取所有母话题的子话题id等信息，
	根据母子话题id等拼接精华话题url去爬取精华话题前几页最精华的话题，
	这样的做法，会爬取到所有话题的精华话题，有质量，有学习参考价值的讨论话题。

### 精华话题推荐思路: ###
	按照数据库精华话题更新时间排序，取出最久的100项话题，
	然后再随机抽取10道精华话题来进行推荐，
	这样的做法，会推荐到最精华的话题，然后又是随机的，保证了无人为参与性。

### 定时发布操作思路： ###
	网站前端使用了wordpress框架，能够快速的搭建前端和创建相应数据库表，后台使用crontab定时执行脚本，
	脚本执行程序，并将打算发表的数据插入到wordpress的文章相关表格中，这样即可在前端看到更新的文章。
	
### 网站使用的技术： ###
    开发语言是c语言，使用curl来爬数据，使用htmlcxx来解析html文件，使用cJson来生成json文件。
    使用redis来缓存，使用hiredis来处理redis，数据库使用mysql，前端网站使用Wordpress。

### 注意事项： ###
	程序依赖的mysql库和爬虫需要的第三方库需要更新为自己系统上的路径，修改Makefile文件中的
	BYCO_HTML_DIR=/home/byco/ZhihuSpider/ZhihuSpider/zhihu_spider/html_common
	INCLUDE += -I/usr/local/mysql/include/ -I${BYCO_HTML_DIR} -I/usr/local/include/hiredis
	LIBS    := -L/usr/local/mysql/lib -lmysqlclient -L/usr/local/lib/ -lhiredis
 	
	安装redis,mysql,hiredis
	db.conf和db_wp.conf连接数据库名密码改为自己的环境下的
	redis_op.cpp默认写死redis的ip和端口为"127.0.0.1" ，6379

### 备注： ###
	由于知乎个人主页信息页结构改动，程序中解析个人主页信息代码有bug，抽空再改。

### 问题和技术交流QQ:
	745226897 
	
### 网站地址和截图
[鸭哥私房菜网站](http://www.smimi.net/ "http://www.smimi.net/") 

![image](https://raw.githubusercontent.com/BycoLin/ZhihuSpider/master/zhihu_spider/images/首页.jpg)

![image](https://raw.githubusercontent.com/BycoLin/ZhihuSpider/master/zhihu_spider/images/话题榜.jpg)

![image](https://raw.githubusercontent.com/BycoLin/ZhihuSpider/master/zhihu_spider/images/风云榜.jpg)

![image](https://raw.githubusercontent.com/BycoLin/ZhihuSpider/master/zhihu_spider/images/统计图.jpg)

![image](https://raw.githubusercontent.com/BycoLin/ZhihuSpider/master/zhihu_spider/images/分布图.jpg)	
	

	
