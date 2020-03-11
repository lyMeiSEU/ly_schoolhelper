package cn.edu.seu.alumni_server.controller;

import cn.edu.seu.alumni_server.common.Utils;
import cn.edu.seu.alumni_server.common.dto.WebResponse;
import cn.edu.seu.alumni_server.common.token.Acl;
import cn.edu.seu.alumni_server.service.QCloudFileManager;
import java.io.IOException;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.multipart.MultipartFile;

@RestController
@Acl
public class CommonController {

	@Autowired
	QCloudFileManager qCloudFileManager;

	@PostMapping("/uploadFile")
	public WebResponse uploadFile(
		@RequestParam MultipartFile file
	) {
		// 首先获取 newName
		String newNameWithoutType = String.valueOf(Utils.generateId());
		String newNameWithType = this.qCloudFileManager.buildNewFileNameWithType(
			file, newNameWithoutType
		);
		String ansUrl = null;
		try {
			ansUrl = qCloudFileManager.uploadOneFile(
				file,
				newNameWithoutType
			);
		} catch (IOException e) {
			return new WebResponse().fail("上传文件失败.");
		}
		// 要删除文件
		Utils.deleteFileUnderProjectDir(newNameWithType);
		// 返回最终结果
		return new WebResponse().success(
			ansUrl
		);
	}

	@DeleteMapping("/deleteFile")
	public WebResponse deleteFile(
		@RequestParam String fileUrl
	) {
		try {
			// 注意这里的删除不检查是否有这个文件, 直接就是删除请求
			this.qCloudFileManager.deleteObject(
				fileUrl.substring(fileUrl.lastIndexOf("/") + 1)
			);
			return new WebResponse<String>().success("删除成功.");
		} catch (IOException e) {
			return new WebResponse().fail("删除失败.");
		}
	}

}
