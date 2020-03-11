package cn.edu.seu.alumni_server.service.fail;

import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Objects;
import java.util.Properties;
import lombok.Getter;

@Getter
public abstract class ServiceFailPrompt {
	protected Map<Integer, String> failCodeToUserPrompt;
	protected Map<String, Integer> userPromptToFailCode;
	protected String relativePath;

	public void configByProperties(String relativePath) throws IOException {
		this.failCodeToUserPrompt = new HashMap<>();
		this.userPromptToFailCode = new HashMap<>();
		Properties properties = new Properties();
		properties.load(
			new InputStreamReader(
				Objects.requireNonNull(
					ServiceFailPrompt.class.getClassLoader().getResourceAsStream(relativePath)),
				StandardCharsets.UTF_8
			)
		);
		for (Entry<Object, Object> entry: properties.entrySet()) {
			Integer failCode = Integer.valueOf((String) entry.getKey());
			String userPrompt = (String) entry.getValue();
			this.failCodeToUserPrompt.put(failCode, userPrompt);
			this.userPromptToFailCode.put(userPrompt, failCode);
		}
	}

	public String getUserPrompt(String requestName, Integer failCode) {
		String temp = (
			this.failCodeToUserPrompt.get(failCode) == null ?
				"未知错误" :
				this.failCodeToUserPrompt.get(failCode)
		);
		return String.format(
			"%s请求失败. 失败原因: %s",
			requestName, temp
		);
	}

	public Integer getFailCode(String userPrompt) {
		return (
			this.userPromptToFailCode.get(userPrompt) == null ?
				-1 :
				this.userPromptToFailCode.get(userPrompt)
		);
	}
}
