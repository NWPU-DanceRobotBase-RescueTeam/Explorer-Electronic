git init	//初始化
git add	//添加文件
git status	//仓库当前状态
git commit -m "备注"   //仓库更新
git rm 文件.后缀	//删除文件
git rm -r 文件夹	//删除文件夹
git clone 地址	//克隆远程库
git remote add origin 地址 	//连接远程库（将远程库地址命名为origin，也可命名为其他）
git pull origin master	(分支名称)		//将当前分支master分支上的所有远程仓库中的代码下载到本地仓库
git push origin master(分支名称)	//将当前分支的所有本地仓库中的改动提交到远程仓库中
git remote -v  		//查看远程仓库连接情况
git config --global credential.helper store 		//github长期储存密码
git remote remove origin 	//移除当前的远程库
