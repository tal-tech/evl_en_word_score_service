# 英文单词语音评测
进入src目录中，使用命令sudo bash requirements.sh，安装相关依赖<br/>
更换测试的样例，修改src/det_evl_eng/task.cpp中check_decode_audio的m_file_name<br/>
更换要使用的模型，修改src/det_evl_eng/evl.engeneral.cpp<br/>

## Install
```
cd ./src/
cmake CMakeLists.txt
make
```
The executable file is in ./bin/<br/>
```
./GodeyeEvlENG # run program
```