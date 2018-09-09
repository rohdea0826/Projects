#include <string>
#include <vector>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <curl/curl.h>

//文章URL
const std::string MainURL = "http://www.readers365.com/World/001/";
//"http://ds.eywedu.com";
//文章目录正则表达式
const std::string Catalog = "ts\\S+htm";
//章节标题起始/结束标志
const std::string Title_Flag_begin = "<font size=\"4\">";
const std::string Title_Flag_end = "</font></b></p>";
//章节正文起始/结束标志
const std::string Content_Flag_begin = "\"></script></DIV>"; 
const std::string Content_Flag_end = "<!--/HTMLBUILERPART0-->";


//ptr:libcurl接受到的相应的html页面的存储缓冲区
//size * nmemb:普通人缓冲区中数据的长度
//userdata：用户自定义的一个缓冲区
size_t WriteData(char*ptr, size_t size, size_t nmemb, void* userdata)
{
	std::string* output = (std::string*)userdata;
	output->append(ptr, size * nmemb);
	return size * nmemb;
}

class ScopedHandler
{
public:
	ScopedHandler(CURL* h) : handler(h){}
	~ScopedHandler(){curl_easy_cleanup(handler);}
private:
	CURL* handler;
};

bool OpenPage(const std::string& url, std::string* html)
{
	//初始化句柄
	CURL* handler = curl_easy_init();
	ScopedHandler scoped_handler(handler);
	//构造HTTP请求，最核心的就是设置URL。
	//HTTP请求中的其他header，libcurl也是支持的。
	//当前我们不需要
	curl_easy_setopt(handler, CURLOPT_URL, url.c_str());
	//设置响应应该如何处理
	curl_easy_setopt(handler, CURLOPT_WRITEFUNCTION, WriteData);
	curl_easy_setopt(handler, CURLOPT_WRITEDATA, html);
	//发送请求
	CURLcode ret = curl_easy_perform(handler);
	if(ret != CURLE_OK)
	{	
		fprintf(stderr, "curl_easy_perform failed!\n");
		return false;
	}
	return true;
}

/* void test1()
{
	std::string html;
	OpenPage(URL, &html);
	printf("%s\n",html.c_str());
} */

void ParseMainPage(const std::string& html, std::vector<std::string>* url_list)
{
	boost::regex reg(Catalog);
	//借助reg对象进行查找
	std::string::const_iterator cur = html.begin();
	std::string::const_iterator end = html.end();
	boost::smatch result;
	while(boost::regex_search(cur, end, result, reg))
	{
		//result[0]为boost中内置的对象，可以隐式转换成string
		//从这个对象中取到second属性，就对应了接下来要查找的开始位置
		url_list->push_back(MainURL + result[0]);
		cur = result[0].second;
	}
}

/* void test2()
{
	std::string html;
	OpenPage(MainURL, &html);
	std::vector<std::string> url_list;
	ParseMainPage(html, &url_list);
	for(size_t i = 0; i < url_list.size(); i++)
	{
		printf("%s\n", url_list[i].c_str());
	}
} */

/* void test3()
{
	std::string html;
	OpenPage(, &html);
	printf("%s\n", html.c_str());	
} */


void ParseDetailPage(const std::string& html, std::string* content)
{
     //标题
     size_t beg = html.find(Title_Flag_begin);
     if(beg == std::string::npos)
     {
         fprintf(stderr, "找不到开始标志\n");
         return ;
     }
     beg += Title_Flag_begin.size();
 
     size_t end = html.find(Title_Flag_end);
     if(end == std::string::npos)
     {
         fprintf(stderr,"找不到结束标志\n");
         return;
     }
     if(beg >= end)
     {
         fprintf(stderr,"标志错误\n");
         return;
     }
    *content = html.substr(beg, end - beg);
	*content += "\n";

	//正文
	beg = html.find(Content_Flag_begin);
	if(beg == std::string::npos)
	{
		fprintf(stderr, "找不到开始标志\n");
		return ;
	}
	beg += Content_Flag_begin.size();
	
	end = html.find(Content_Flag_end);
	if(end == std::string::npos)
	{
        fprintf(stderr,"找不到结束标志\n");
		return;
	}
	if(beg >= end)
	{
		fprintf(stderr,"标志错误\n");
		return;
	}
	*content += html.substr(beg, end - beg);
	//替换掉转义字符
	boost::algorithm::replace_all(*content, "&nbsp;", " ");
	boost::algorithm::replace_all(*content, "<BR>", " ");	
    boost::algorithm::replace_all(*content, "<h1>", " ");
    boost::algorithm::replace_all(*content, "</h1>", " ");

}

/* void test4()
{
    FILE *fd = fopen("./out.txt","a");
	std::string html;
	OpenPage(, &html);
	std::string content;
	ParseDetailPage(html, &content);
	fprintf(fd,"%s\n", content.c_str());
} */

void run()
{
    FILE *fd = fopen("./out.txt","w");
	std::string html;
    OpenPage(MainURL, &html);
    std::vector<std::string> url_list;
    ParseMainPage(html, &url_list);
    printf("mainPage sucess!\n");
    for(size_t i = 0; i < url_list.size(); i++)
    {
        printf("%s\n", url_list[i].c_str());
        OpenPage(url_list[i].c_str(), &html);
        std::string content;
        ParseDetailPage(html, &content);
        fprintf(fd,"%s\n", content.c_str());
        content.clear();
        html.clear();
    }
    fclose(fd);
}

int main()
{
	run();
	//test1();
	//test2();
	//test3();
	//test4();
	return 0;
}
