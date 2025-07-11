// CONFIGURATION (MUST MATCH YOUR SERVER)
const API_BASE = 'http://localhost:8080';  // ← ️VERIFY THIS PORT!
const POLL_INTERVAL = 1000; // ms

class SchedulerVisualizer {
  constructor() {
    this.activeRequests = new Set();
    this.init();
  }

  init() {
    console.log("Initializing scheduler visualizer...");
    this.initDOM();
    this.setupEventListeners();
    this.startPolling();
    this.testConnection();
  }

  initDOM() {
    this.dom = {
      processContainer: document.getElementById('process-container'),
      runningDisplay: document.getElementById('running-pid'),
      addBtn: document.getElementById('add-btn'),
      pidInput: document.getElementById('pid-input'),
      burstInput: document.getElementById('burst-input')
    };
  }

  setupEventListeners() {
    this.dom.addBtn.addEventListener('click', () => this.addProcess());
  }

  // ========================
  // DEBUGGING TOOLS
  // ========================
  async testConnection() {
    try {
      const start = Date.now();
      const response = await this.fetchWithTimeout(`${API_BASE}/api/health`, {}, 2000);
      console.log(`✅ Backend connection successful (${Date.now() - start}ms)`);
      return true;
    } catch (error) {
      console.error("❌ BACKEND CONNECTION FAILED", {
        error: error.message,
        checks: [
          `1. Server running at ${API_BASE}?`,
          "2. '/api/health' endpoint implemented?",
          "3. Check browser console for CORS errors"
        ]
      });
      return false;
    }
  }

  // ========================
  // ENHANCED FETCH METHODS
  // ========================
  async fetchWithTimeout(url, options = {}, timeout = 5000) {
    const controller = new AbortController();
    const timeoutId = setTimeout(() => controller.abort(), timeout);
    
    try {
      const requestId = Symbol();
      this.activeRequests.add(requestId);
      
      const response = await fetch(url, {
        ...options,
        signal: controller.signal,
        headers: {
          'Accept': 'application/json',
          ...(options.headers || {})
        }
      });
      
      clearTimeout(timeoutId);
      this.activeRequests.delete(requestId);
      
      if (!response.ok) throw new Error(`HTTP ${response.status}`);
      return response;
    } catch (error) {
      clearTimeout(timeoutId);
      this.activeRequests.delete(requestId);
      throw error;
    }
  }

  // ========================
  // API COMMUNICATION (FIXED)
  // ========================
  async fetchState() {
    try {
      const response = await this.fetchWithTimeout(`${API_BASE}/api/state`);
      const data = await response.json();
      console.debug("State update:", data);
      return data;
    } catch (error) {
      console.warn("State fetch failed:", error.message);
      return null;
    }
  }

  async addProcess() {
    const pid = this.dom.pidInput.value;
    const burst = this.dom.burstInput.value;

    if (!pid || !burst) return;

    try {
      const response = await this.fetchWithTimeout(`${API_BASE}/api/process`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ pid, burst })
      });

      console.log(`Process added (PID: ${pid})`);
      this.dom.pidInput.value = '';
      this.dom.burstInput.value = '';
      this.forceUpdate();
    } catch (error) {
      console.error("Failed to add process:", error);
    }
  }

  // ========================
  // RELIABLE POLLING SYSTEM
  // ========================
  startPolling() {
    let isUpdating = false;

    const poll = async () => {
      if (isUpdating) {
        console.warn("Skipping poll - previous update still in progress");
        return;
      }

      isUpdating = true;
      try {
        await this.update();
      } catch (error) {
        console.error("Polling error:", error);
      } finally {
        isUpdating = false;
      }
    };

    // Initial immediate update
    poll();
    
    // Regular polling
    this.pollInterval = setInterval(poll, POLL_INTERVAL);
  }

  async update() {
    const state = await this.fetchState();
    if (state) this.render(state);
  }

  forceUpdate() {
    clearInterval(this.pollInterval);
    this.startPolling();
  }

  render(state) {
    this.dom.runningDisplay.textContent = state.running 
      ? `PID: ${state.running.pid}` 
      : "IDLE";

    this.dom.processContainer.innerHTML = state.queue.map(proc => `
      <div class="process ${proc.pid === state.running?.pid ? 'active' : ''}">
        <div class="pid">${proc.pid}</div>
        <div class="time">${proc.timeLeft}ms</div>
      </div>
    `).join('');
  }
}

// Initialize with error handling
try {
  document.addEventListener('DOMContentLoaded', () => {
    new SchedulerVisualizer();
  });
} catch (error) {
  console.error("Initialization failed:", error);
}