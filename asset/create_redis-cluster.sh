#! /bin/bash

# docker 安装的验证
# 删除既有安装的redis容器
# 删除既有的redis镜像, 获取最新redis镜像
# 端口的验证
# 创建redis容器的配置信息
# 创建redis镜像
# 生成配置redis集群的配置点信息
# 启动redis集群的开始节点

# Windows 10 gitbash下创建 Redis集群脚本
echo '============================================================'
echo 'Windows 10 gitbash下创建 Redis集群脚本'
echo `date`
echo '============================================================'

# 需要应用的端口定义
PORTS_START=4219 # 10端口, 5主5从
PORTS_END=4228

PORTS=`seq ${PORTS_START} ${PORTS_END}` # 10端口, 5主5从
REDIS_POD_PREFIX='g_redis'
# REDIS_DOCKER_PATH='D:\xg\workspace\cplusplus\redis-cluster'
REDIS_DOCKER_PATH=`pwd`

# Redis集群用用户配置模板
CLUSTER_CONFIG_TEMPLATE='
# Docker 用 Redis集群用户自定义配置文件
\n
\nport ${PORT}
\nprotected-mode no
\ncluster-enabled yes
\ncluster-config-file nodes.conf
\ncluster-node-timeout 5000
\n# 自己服务器IP
\ncluster-announce-ip 192.168.3.235
\ncluster-announce-port ${PORT}
\ncluster-announce-bus-port 1${PORT}
\nappendonly yes'

# 检查 docker 是否已经安装
function fn_check_docker_install_and_exit() {
  docker --version > /dev/null 2>&1

  erro=$?
  if [ ${erro} != 0 ]
  then
    echo -e '>> \e[1;31m当前系统未安装 docker\e[0m'
    exit 1
  fi

  echo '>> 当前安装 docker 版本信息'
  echo -e `docker --version`'\n'

  return 0
}

# 获取 redis docker
function fn_pull_redis_image_and_exit() {
  echo '>> 获取最新 redis 镜像'
  # 先删除再拉取
  docker rmi redis > /dev/null 2>&1
  docker pull redis

  # 再检查一次
  docker images | grep 'redis' > /dev/null 2>&1

  erro=$?
  if [ ${erro} != 0 ]
  then
    echo '>> 获取 redis 镜像失败'
    exit 1
  fi

  echo '>> 当前安装 redis 镜像信息'
  echo -e `docker images | grep 'redis'`'\n'

  return 0
}

# 删除已经存在的容器
function fn_delete_redis_container() {
  docker ps -a                   \
    | grep "${REDIS_POD_PREFIX}" \
    | cut -d ' ' -f 1            \
    | xargs docker rm -f > /dev/null 2>&1
}

function fn_redis_cluster_config() {
  port=$1
  echo "  创建 Redis集群 端口 ${port} 的配置内容"

  # 生成 conf|data目录
  mkdir -p ./${port}/conf
  mkdir -p ./${port}/data

  # 删除既存数据
  rm -rf ./${port}/data/*

  # 生成Docker 用 Redis集群用户自定义配置文件
  echo -e ${CLUSTER_CONFIG_TEMPLATE} | \
    PORT=${port} envsubst > ./${port}/conf/redis.conf

  return 0
}

# 检查端口的可用性
function fn_check_port_and_exit() {
  for port in ${PORTS}
  do
    echo -e ">> 测试端口\e[1;32m ${port} \e[0m的可用性"
    sleep 2s
    netstat -ano | grep ${port} | grep 'WAIT' -v
    erro=$?

    if [ ${erro} == "0" ]
    then
      echo -e "  << 端口\e[1;31m ${port} \e[0m已占用"
      exit 1
    fi

    # 生成Docker 用 Redis集群用户自定义配置文件
    fn_redis_cluster_config ${port}

  done

  return 0
}

# 创建 Redis集群 容器
function fn_create_container() {
  redis_server_conf='/usr/local/etc/redis/redis.conf'
  echo -e '\n>> 创建 Redis集群 容器'
  for port in ${PORTS}
  do
    docker run -dit                                                         \
      -p ${port}:${port} -p 1${port}:1${port}                               \
      -v /${REDIS_DOCKER_PATH}/${port}/conf/redis.conf:${redis_server_conf} \
      -v /${REDIS_DOCKER_PATH}/${port}/data:/data                           \
      --name ${REDIS_POD_PREFIX}-${port}                                    \
      --hostname ${REDIS_POD_PREFIX}-host-${port}                           \
      --restart always                                                      \
      --sysctl net.core.somaxconn=1024 redis redis-server /${redis_server_conf}
  done

  return 0
}

# 得到集群网络中的容器ip地址
function fn_get_container_ip_address() {
  echo -e '\n\e[1;32m>> 拷贝以下内容, 在输出的提示符下执行(配置 redis 集群)\e[0m'
  echo -n 'redis-cli --cluster create '
  for port in ${PORTS}
  do
    echo -n "$(docker inspect --format="{{.NetworkSettings.IPAddress }}" "${REDIS_POD_PREFIX}-${port}")":${port} ' '
  done

  echo ' --cluster-replicas 1'
  return 0
}

# 检查 docker 是否已经安装
fn_check_docker_install_and_exit

# 删除已经存在的容器
fn_delete_redis_container

# 获取 redis docker
fn_pull_redis_image_and_exit

# 检查端口的可用性
fn_check_port_and_exit

# 创建 Redis集群 容器
fn_create_container

# 得到集群网络中的容器ip地址
fn_get_container_ip_address

# 启动集群的一个节点, 准备配置集群
docker exec -it ${REDIS_POD_PREFIX}-${PORTS_START} bash
