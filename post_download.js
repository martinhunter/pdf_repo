import { Component } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';

@Component({
  selector: 'app-download',
  templateUrl: './download.component.html'
})
export class DownloadComponent {
  constructor(private http: HttpClient) {}

  downloadFile() {
    const url = '/download'; // 替换为实际的后端路由

    const requestData = { file_id: 'your_file_id' }; // 替换为实际的参数

    // 设置HTTP请求头
    const httpOptions = {
      headers: new HttpHeaders({ 'Content-Type': 'application/json' }),
      responseType: 'blob' as 'json'
    };

    // 发起POST请求
    this.http.post(url, requestData, httpOptions).subscribe((response: any) => {
      const blob = new Blob([response], { type: 'application/octet-stream' });
      const url = window.URL.createObjectURL(blob);

      // 创建一个下载链接
      const a = document.createElement('a');
      a.href = url;
      a.download = 'downloaded_file_name.ext'; // 替换为实际的文件名
      a.style.display = 'none';

      document.body.appendChild(a);

      // 触发下载
      a.click();

      // 释放资源
      window.URL.revokeObjectURL(url);
    });
  }
}
