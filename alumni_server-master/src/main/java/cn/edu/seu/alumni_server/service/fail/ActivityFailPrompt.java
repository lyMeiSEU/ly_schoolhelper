package cn.edu.seu.alumni_server.service.fail;

import java.io.IOException;

public class ActivityFailPrompt extends ServiceFailPrompt {

	public ActivityFailPrompt(String relativePath) throws IOException {
		this.relativePath = relativePath;
		this.configByProperties(relativePath);
	}
}
