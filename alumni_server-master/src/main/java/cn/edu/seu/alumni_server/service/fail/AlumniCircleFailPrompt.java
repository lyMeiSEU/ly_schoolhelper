package cn.edu.seu.alumni_server.service.fail;

import java.io.IOException;

public class AlumniCircleFailPrompt extends ServiceFailPrompt {

	public AlumniCircleFailPrompt(String relativePath) throws IOException {
		this.relativePath = relativePath;
		this.configByProperties(this.relativePath);
	}
}
