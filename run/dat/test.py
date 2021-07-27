from datetime import datetime
from contextlib import closing
from tqsdk import TqApi, TqSim, TqAuth
from tqsdk.tools import DataDownloader

api = TqApi(auth=TqAuth("asialine", "Test518918ok"))
download_tasks = {}

download_tasks["T_tick"] = DataDownloader(api, symbol_list=["CFFEX.T1509"], dur_sec=0,
                    start_dt=datetime(2013, 5, 1), end_dt=datetime(2019, 6, 1), csv_file_name="T1509_tick.csv")
# 使用with closing机制确保下载完成后释放对应的资源
with closing(api):
    while not all([v.is_finished() for v in download_tasks.values()]):
        api.wait_update()
        print("progress: ", { k:("%.2f%%" % v.get_progress()) for k,v in download_tasks.items() })
