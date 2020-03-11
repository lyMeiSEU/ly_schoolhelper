package cn.edu.seu.alumni_server.service.fail;

import java.io.IOException;

public class ActivityMemberFailPrompt extends ServiceFailPrompt {
	public ActivityMemberFailPrompt(String relativePath) throws IOException {
		this.relativePath = relativePath;
		this.configByProperties(relativePath);
	}
}
