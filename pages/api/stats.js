export default async function handler(req, res) {
  const { apiKey } = req.query;

  try {
    const response = await fetch("https://api.hackatime.org/v1/stats", {
      headers: { Authorization: `Bearer ${apiKey}` }
    });
    const data = await response.json();
    res.status(200).json(data);
  } catch (err) {
    res.status(500).json({ error: "Failed to fetch HackaTime data" });
  }
}
